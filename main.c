#include "equipements/equipement.h"
#include "graphe/graphe.h"
#include "graphe/algos.h"
#include "reseau/reseau.h"
#include "trame/trame.h"
#include <stdio.h>



int main() {

    reseau r;
    read_conf("test.txt", &r);
    afficher_reseau(&r);
    deinit_reseau(&r);

    return 0;
}


