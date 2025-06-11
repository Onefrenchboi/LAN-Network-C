#include "equipements/equipement.h"
#include "graphe/graphe.h"
#include "graphe/algos.h"
#include "reseau/reseau.h"
#include "trame/trame.h"
#include <stdio.h>



int main() {




    

    // print_equipement((equipement*)sta);
    // printf("\n");
    // print_equipement((equipement*)sw);
    // printf("\n");
    // print_trame_ethernet(t);
    // print_trame_ethernet(t2);


    reseau r;
    read_conf("conf.txt", &r);


    MAC targetpour7 = read_mac_from_str("54:d6:a6:82:c5:01");
    MAC targetpour8 = read_mac_from_str("54:d6:a6:82:c5:02");
    trame_ethernet t = creer_trame_vide(r.equipements[7], targetpour8);
    trame_ethernet t2 = creer_trame_vide(r.equipements[8], targetpour7);
    envoyer_trame(r.equipements[7], &t);
    printf("\n\n\n\n");
    envoyer_trame(r.equipements[8], &t2);


    
    afficher_reseau(&r);
    deinit_reseau(&r);

    return 0;
}


