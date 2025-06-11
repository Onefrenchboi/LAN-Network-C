#include "equipements/equipement.h"
#include "graphe/graphe.h"
#include "graphe/algos.h"
#include "reseau/reseau.h"
#include "trame/trame.h"
#include <stdio.h>



int main() {
    reseau r;
    read_conf("conf.txt", &r);


    MAC targetpour7 = read_mac_from_str("54:d6:a6:82:c5:01");
    MAC targetpour8 = read_mac_from_str("54:d6:a6:82:c5:02");
    MAC targetpour14 = read_mac_from_str("54:d6:a6:82:c5:08");
    MAC targetBROAD = read_mac_from_str("FF:FF:FF:FF:FF:FF");
    trame_ethernet t = creer_trame_vide(r.equipements[7], targetBROAD);
    trame_ethernet t1 = creer_trame_vide(r.equipements[7], targetpour7);
    trame_ethernet t2 = creer_trame_vide(r.equipements[8], targetpour7);
    trame_ethernet t3 = creer_trame_vide(r.equipements[14], targetpour7);
    
    envoyer_trame(r.equipements[7], &t);
    printf("\n\n\n\n\n\n\n\n");
    envoyer_trame(r.equipements[7], &t1);
    printf("\n\n\n\n\n\n\n\n");
    envoyer_trame(r.equipements[8], &t2);
    printf("\n\n\n\n\n\n\n\n");
    envoyer_trame(r.equipements[14], &t3);
    
    
    afficher_reseau(&r);
    deinit_reseau(&r);

    return 0;
}


