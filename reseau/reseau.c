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
            st->base.type = STATION;
            st->base.addr_MAC=read_mac_from_str(mac);
            st->addr_IP=read_ip_from_str(ip);
            reseau->equipements[i] = (equipement*)st;
        } else if (type == 2) { //switch
            if (fscanf(f, "%17[^;];%d;%d", mac, &nb_ports, &priority) != 3) {
                fclose(f);
                fprintf(stderr, "tu sais pas lire les switch");
                return;
            }
            switch_t* sw = malloc(sizeof(switch_t));
            sw->base.type = SWITCH;
            sw->base.addr_MAC=read_mac_from_str(mac);
            sw->nb_ports = nb_ports;
            sw->priority = priority;
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
        reseau->liens[i].s1 = reseau->equipements[idx1];
        reseau->liens[i].s2 = reseau->equipements[idx2];
        reseau->liens[i].poids = poids;
    }
    fclose(f);
}



void afficher_reseau(reseau* reseau){
    printf(BOLDRED("Equipements (%zu):\n"), reseau->nb_equipements);
    for (size_t i = 0; i < reseau->nb_equipements; ++i) {
        equipement* e = reseau->equipements[i];
        printf(BOLDWHITE("%zu. "), i);
        print_equipement(e);
        printf("-------------------------\n");
    }

    printf(BOLDRED("Liens (%zu):\n"), reseau->nb_liens);
    for (size_t i = 0; i < reseau->nb_liens; ++i) {
        lien* l = &reseau->liens[i];
        int idx1 = -1, idx2 = -1;
        for (size_t j = 0; j < reseau->nb_equipements; ++j) {
            if (reseau->equipements[j] == l->s1) idx1 = j;
            if (reseau->equipements[j] == l->s2) idx2 = j;
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
    free(reseau->liens);
    reseau->liens = NULL;

    //deiniti le reste tsais
    reseau->nb_equipements = 0;
    reseau->nb_liens = 0;
}