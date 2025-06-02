#include "reseau.h"


void read_conf(char* filename){
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Erreur ouverture fichier, cheh");
        return NULL;
    }

}