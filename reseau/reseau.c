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
                fprintf(stderr, "tu sais pas lire les ");
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
            fprintf(stderr, "tu sais pas lire les liens");
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
            fprintf(stderr, "pu de port dispo");
            free(l);
            fclose(f);
            return;
        }
        

        l->portA = &e1->ports[port1];
        l->portB = &e2->ports[port2];
        l->poids = poids;
        reseau->liens[i] = l;
        
        l->portA->lien = l;
        l->portB->lien = l;
    }
    fclose(f);
}

void afficher_reseau(reseau* reseau){
    printf(BOLDRED("Equipements (%zu):\n"), reseau->nb_equipements);
    for (size_t i = 0; i < reseau->nb_equipements; ++i) {
        equipement* e = reseau->equipements[i];
        printf(BOLDWHITE("%zu. "), i);
        print_equipement(e);
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