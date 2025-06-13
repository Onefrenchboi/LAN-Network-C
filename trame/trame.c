#include "trame.h"
#include <stdio.h>

void print_octets(const octet *tab, size_t taille) {
    printf(YELLOW("%02x"), tab[0]);
    for (size_t i = 1; i < taille; ++i) {
        printf(YELLOW(":%02x"), tab[i]);
    }
}

void print_octets_sans_double_points(const octet *tab, size_t taille) {
    printf(YELLOW("%02x"), tab[0]);
    for (size_t i = 1; i < taille; ++i) {
        printf(YELLOW("%02x"), tab[i]);
    }
}






void print_trame_ethernet(const trame_ethernet *t) {
    if (t == NULL) {
        printf("J'ai dit une trame pas un tram");
        return;
    }
    printf(GREEN("Trame :\n"));
    printf(BOLDWHITE("Préambule : "));
    print_octets(t->preambule, sizeof(t->preambule));
    printf(BOLDWHITE("\nSFD : "));
    printf(YELLOW("%02x"), t->sfd);
    printf(BOLDWHITE("\nDestination : "));
    print_mac(t->destination);
    printf(BOLDWHITE("\nSource : "));
    print_mac(t->source);
    printf(BOLDWHITE("\nType : "));
    printf("%d",t->type);
    printf(BOLDWHITE("\nDonnées : "));
    print_octets(t->data, sizeof(t->data));
    printf(BOLDWHITE("\nBourrage : "));
    print_octets(t->bourrage, sizeof(t->bourrage));
    printf(BOLDWHITE("\nFCS : "));
    print_octets(t->fcs, sizeof(t->fcs));
}

void print_trame_ethernet_brut(const trame_ethernet *t) {
    if (t == NULL) {
        printf("J'ai dit une trame pas un tram");
        return;
    }
    printf(GREEN("Trame :\n"));
    print_octets_sans_double_points(t->preambule, sizeof(t->preambule));
    printf(YELLOW("%02x"), t->sfd);
    print_mac_sans_double_points(t->destination);
    print_mac_sans_double_points(t->source);
    printf("%d",t->type);
    print_octets_sans_double_points(t->data, sizeof(t->data));
    print_octets_sans_double_points(t->bourrage, sizeof(t->bourrage));
    print_octets_sans_double_points(t->fcs, sizeof(t->fcs));
}


trame_ethernet creer_trame_vide(equipement* e, MAC target) {
    trame_ethernet trame;

    //préambule synchronisation 7 octets 10101010 (début peut être perdu)
    memset(trame.preambule, 0xAA, 7);

    //SFD (Start of Frame Delimiter) : début de l’info utile 1 octet 10101011
    trame.sfd = 0xAB;

    //adresse destination : 6 octets 
    memcpy(&trame.destination, &target, sizeof(MAC)); //On copie l'adresse MAC de destination dans la trame

    //adresse source : 6 octets
    memcpy(&trame.source, &e->addr_MAC, sizeof(MAC)); //On copie l'adresse MAC de l'équipement dans la trame
    //type : ethernet ou BPDU
    trame.type = ETHERNET_T; //On initialise le type de la trame à Ethernet, on peut changer plus tard si besoin

    //DATA (données) : 46 à 1500 octets (y compris bourrage/padding éventuel)
    memset(trame.data, 0, 1500); //Ici on va mettre tout a 0 
    memset(trame.bourrage, 0, 46); 

    //FCS (Frame Check Sequence) : 4 octets - code polynomial détecteur d’erreur
    memset(trame.fcs, 0, 4);

    return trame;
}
    


trame_ethernet creer_trame_BPDU(equipement* e){
    
    trame_ethernet trame = creer_trame_vide(e, read_mac_from_str("01:80:c2:00:00:00")); //Adresse MAC de destination pour les BPDU

    //On met le type de la trame à BPDU
    trame.type = BDPU_T;

    //On remplit les données avec les données de la BPDU
    BPDU b;
    b.root = e->addr_MAC;
    b.cost = 0;
    b.bridge_id = e->addr_MAC;

    memcpy(trame.data, &b, sizeof(BPDU));
    return trame;
}

bool is_BPDU(trame_ethernet* trame) {
    //On vérifie si le type de la trame est bpdu
    if (trame->type==BDPU_T) {
        return true;
    }
    return false;
}

BPDU read_BPDU_from_trame(trame_ethernet* trame) {
    BPDU b;
    if (is_BPDU(trame)) {
        memcpy(&b, trame->data, sizeof(BPDU));
    } else {
        memset(&b, 0, sizeof(BPDU));
    }
    return b;
}

bool compare_BPDU(BPDU* b1, BPDU* b2) {
    if (memcmp(&b1->root, &b2->root, sizeof(MAC)) < 0) {
        return true; //b1 est meilleur
    }
    if (memcmp(&b1->root, &b2->root, sizeof(MAC)) < 0) {
        return true; //b1 est meilleur
    }
    if (b1->cost < b2->cost) {
        return true; //b1 est meilleur
    }

    return false; //b2 est meilleur ou égal
}