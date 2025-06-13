#include "reseau.h"


void read_conf(char* filename, reseau* reseau){

    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Erreur ouverture fichier, cheh");
        return;
    }
    
    size_t nb_equipements, nb_aretes;
    if (fscanf(f, "%zu %zu\n", &nb_equipements, &nb_aretes) != 2) {
        fclose(f);
        fprintf(stderr, "tu sais pas lire bouuuuh");
        return;
    }   

    reseau->nb_equipements = nb_equipements;
    reseau->nb_liens = nb_aretes;
    reseau->equipements = malloc(nb_equipements * sizeof(equipement*));
    reseau->liens = malloc(nb_aretes * sizeof(lien));

    //lecture des équipements
    for (size_t i = 0; i < nb_equipements; i++) {
        int type;
        char mac[100], ip[100]; //!temp, change
        int nb_ports, priority;

        //on lit la première valeur pour savoir le type
        if (fscanf(f, "%d;", &type) != 1) {
            fclose(f);
            fprintf(stderr, "tu sais toujours pas lire bouuuuh");
            return;
        }  
        

        if (type == 1) { //machine
            if (fscanf(f, "%17[^;];%12[^;\n]", mac, ip) != 2) {
                fclose(f);
                fprintf(stderr, "tu sais pas lire les machines");
                return;
            }
            station_t* st = malloc(sizeof(station_t));
            init_station(st, mac, ip);
            reseau->equipements[i] = (equipement*)st;
        } else if (type == 2) { //switch
            if (fscanf(f, "%17[^;];%d;%d", mac, &nb_ports, &priority) != 3) {
                fclose(f);
                fprintf(stderr, "tu sais pas lire les switch");
                return;
            }
            switch_t* sw = malloc(sizeof(switch_t));
            init_switch(sw, mac, nb_ports, priority);
            reseau->equipements[i] = (equipement*)sw;
        }
    }


    //lecture des arêtes
    for (size_t i = 0; i < nb_aretes; ++i) {
        int idx1, idx2, poids;
        if (fscanf(f, "%d;%d;%d\n", &idx1, &idx2, &poids) != 3) {
            fclose(f);
            printf(RED("tu sais pas lire les liens"));
            return;
        }


        lien* l = malloc(sizeof(lien));
        equipement* e1 = reseau->equipements[idx1];
        equipement* e2 = reseau->equipements[idx2];
        size_t port1 = (size_t)-1; 
        size_t port2 = (size_t)-1;



        for (size_t y = 0; y < e1->nb_ports; y++) {
            if (e1->ports[y].lien == NULL) {
                port1 = y;
                break;
            }
        }
        for (size_t y = 0; y < e2->nb_ports; y++) {
            if (e2->ports[y].lien == NULL) {
                port2 = y;
                break;
            }
        }
        if (port1 == (size_t)-1 || port2 == (size_t)-1) {
            printf(RED("a pu de ports\n"));
            free(l);
            fclose(f);
            return;
        }
        

        l->portA = &e1->ports[port1];
        l->portB = &e2->ports[port2];
        l->poids = poids;
        
        l->portA->lien = l;
        l->portB->lien = l;
        reseau->liens[i] = l;
    }

    fclose(f);
}

void afficher_reseau(reseau* reseau){
    printf(BOLDRED("Equipements (%zu):\n"), reseau->nb_equipements);
    for (size_t i = 0; i < reseau->nb_equipements; ++i) {
        equipement* e = reseau->equipements[i];
        printf(BOLDWHITE("%zu. "), i);
        print_equipement(e);

        if (e->type == SWITCH) {
            print_ports(e);
        }
        printf("\n-------------------------\n");
    }

    printf(BOLDRED("Liens (%zu):\n"), reseau->nb_liens);
    for (size_t i = 0; i < reseau->nb_liens; ++i) {
        lien* l = reseau->liens[i];
        int idx1 = -1, idx2 = -1;
        for (size_t j = 0; j < reseau->nb_equipements; ++j) {
            if (reseau->equipements[j] == l->portA->parent) {
                idx1 = j;
            }
            if (reseau->equipements[j] == l->portB->parent) {
                idx2 = j;
            }
        }
        printf(MAGENTA("  Lien %zu : "),i);
        printf(YELLOW("%d <-> %d "),  idx1, idx2);
        printf("(poids: %d)\n", l->poids);
    }
}

void deinit_reseau(reseau *reseau)
{
    //free de tout les equipements
    for (size_t i = 0; i < reseau->nb_equipements; ++i) {
        if (reseau->equipements[i]->type == SWITCH) {
            switch_t* sw = (switch_t*)reseau->equipements[i];
            if (sw->bpdu != NULL) {
                free(sw->bpdu);
                sw->bpdu = NULL;
            }
        }
        free(reseau->equipements[i]);
    }
    //free le tableau
    free(reseau->equipements);
    reseau->equipements = NULL;

    //free les liens
    for (size_t i = 0; i < reseau->nb_liens; ++i) {
        free(reseau->liens[i]);
    }
    free(reseau->liens);
    reseau->liens = NULL;

    //deiniti le reste tsais
    reseau->nb_equipements = 0;
    reseau->nb_liens = 0;
}


void STP(reseau* r) {
    bool changed=true;

    BPDU last_bpdu[r->nb_equipements]; 

    while (changed) {
        //on save els BPDU de tt le mone
        for (size_t i = 0; i < r->nb_equipements; i++) {
            if (r->equipements[i]->type != SWITCH) continue;
            switch_t* sw = (switch_t*)r->equipements[i];
            memcpy(&last_bpdu[i].root, &sw->bpdu->root, sizeof(MAC));
            last_bpdu[i].cost = sw->bpdu->cost;
            memcpy(&last_bpdu[i].bridge_id, &sw->bpdu->bridge_id, sizeof(MAC));
        }

        //tt le monde envoie son BPDU
        for (size_t i = 0; i < r->nb_equipements; i++) {
            if (r->equipements[i]->type != SWITCH) continue;
            send_BPDU(r->equipements[i]);
        }

        //tout le monde s'updaate
        for (size_t i = 0; i < r->nb_equipements; i++) {
            if (r->equipements[i]->type != SWITCH) continue;
            switch_t* sw = (switch_t*)r->equipements[i];

            //on trouve le root
            int root_port = -1;
            int best_cost = 9999999; //valeur arbitraire, c'est juste pour faire grand 
            for (size_t p = 0; p < sw->base.nb_ports; p++) {
                port* pt = &sw->base.ports[p];
                if (pt->lien && pt->status != 'B' && pt->role != 'D') {
                    if (sw->bpdu->cost < best_cost) {
                        best_cost = sw->bpdu->cost;
                        root_port = p;
                    }
                }
            }
            for (uint8_t p = 0; p < sw->base.nb_ports; p++) {
                port* pt = &sw->base.ports[p];
                if (p == root_port) {
                    pt->role = 'R';
                    pt->status = 'F';
                } else if (pt->role != 'D') {
                    pt->role = 'B';
                    pt->status = 'B';
                }
            }
        }

        //pour chaque switch, on met à jour les ports désignés
        for (size_t i = 0; i < r->nb_equipements; i++) {
            if (r->equipements[i]->type != SWITCH){
                continue;
            } 
            switch_t* sw = (switch_t*)r->equipements[i];

            if (memcmp(&sw->bpdu->root, &sw->base.addr_MAC, sizeof(MAC)) == 0){
                continue;
            }

            for (size_t j = 0; j < sw->base.nb_ports; j++) {
                port *p = &sw->base.ports[j];
                if (!p->lien){
                    continue;
                } 

                equipement *autre = (p->lien->portA->parent == sw->base.ports->parent) ? p->lien->portB->parent : p->lien->portA->parent; //mueheh ternaire

                if (autre->type == STATION) {
                    p->status = 'F';
                    p->role = 'D';
                } else if (autre->type == SWITCH) {
                    switch_t *sw_autre = (switch_t *)autre;
                    for (size_t k = 0; k < sw_autre->base.nb_ports; k++) {
                        port *p_autre = &sw_autre->base.ports[k];
                        if (!p_autre->lien) {
                            continue;
                        }
                        equipement *peer = (p_autre->lien->portA->parent == autre) ? p_autre->lien->portB->parent : p_autre->lien->portA->parent;
                        if (peer == (equipement*)sw) {
                            if (p->role == 'R') {
                                p_autre->status = 'F';
                                p_autre->role = 'D';
                            }
                        }
                    }
                }
            }
        }

        //on vérifie si les BPDU ont changé
        changed = false;
        for (size_t i = 0; i < r->nb_equipements; i++) {
            if (r->equipements[i]->type != SWITCH) {
                continue;
            }
            switch_t* sw = (switch_t*)r->equipements[i];
            if (
                memcmp(&last_bpdu[i].root, &sw->bpdu->root, sizeof(MAC)) != 0 ||
                last_bpdu[i].cost != sw->bpdu->cost ||
                memcmp(&last_bpdu[i].bridge_id, &sw->bpdu->bridge_id, sizeof(MAC)) != 0
            ) {
                changed = true;
                break;
            }
        }

    }
}