#include "trame.h"
#include <stdio.h>

void print_octets(const octet *tab, size_t taille) {
    printf(YELLOW("%02x"), tab[0]);
    for (size_t i = 1; i < taille; ++i) {
        printf(YELLOW("%02x"), tab[i]);
    }
    printf("\n");
}


trame_ethernet read_trame_from_str(char* preambule, char* sfd, 
                                    char* destination, char* source, 
                                    char* type, char* data, 
                                    char* bourrage, char* fcs) {
    trame_ethernet trame = {0};
    sscanf(preambule, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
           &trame.preambule[0], &trame.preambule[1], &trame.preambule[2],
           &trame.preambule[3], &trame.preambule[4], &trame.preambule[5],
           &trame.preambule[6]);
    sscanf(sfd, "%02hhx", &trame.sfd);
    trame.destination = read_mac_from_str(destination);
    trame.destination = read_mac_from_str(source);
    sscanf(type, "%02hhx:%02hhx", &trame.type[0], &trame.type[1]);
    for (size_t i = 0; i < sizeof(trame.data); ++i) {
        trame.data[i] = 0x00;
        if (*(data + i * 3) == '\0') {
            break;
        }
        else {
            sscanf(data + i * 3, "%02hhx", &trame.data[i]);
        }
    }
    for (size_t i = 0; i < sizeof(trame.bourrage); ++i) {
        sscanf(bourrage + i * 3, "%02hhx", &trame.bourrage[i]);
    }
    sscanf(fcs, "%02hhx:%02hhx:%02hhx:%02hhx", &trame.fcs[0], &trame.fcs[1], &trame.fcs[2], &trame.fcs[3]);
    return trame;
}




void print_trame_ethernet(const trame_ethernet *t) {
    if (t == NULL) {
        printf("J'ai dit une trame pas un tram");
        return;
    }
    printf(GREEN("Trame :\n"));
    printf(BOLDWHITE("Préambule : "));
    print_octets(t->preambule, sizeof(t->preambule));
    printf(BOLDWHITE("SFD : "));
    printf(YELLOW("%02x\n"), t->sfd);
    printf(BOLDWHITE("Destination : "));
    print_mac(t->destination);
    printf(BOLDWHITE("Source : "));
    print_mac(t->source);
    printf(BOLDWHITE("Type : "));
    print_octets(t->type, sizeof(t->type));
    printf(BOLDWHITE("Données : "));
    print_octets(t->data, sizeof(t->data));
    printf(BOLDWHITE("Bourrage : "));
    print_octets(t->bourrage, sizeof(t->bourrage));
    printf(BOLDWHITE("FCS : "));
    print_octets(t->fcs, sizeof(t->fcs));
}

trame_ethernet creer_trame_vide(equipement* e, MAC target) {
    trame_ethernet trame;

    //préambule synchronisation 7 octets 10101010 (début peut être perdu)
    memset(trame.preambule, 0xAA, 7);

    //SFD (Start of Frame Delimiter) : début de l’info utile 1 octet 10101011
    trame.sfd = 0xAB;

    //adresse destination : 6 octets 
    trame.destination = target; 

    //adresse source : 6 octets
    trame.source = e->addr_MAC; //On met l'adresse MAC de l'équipement qui envoie la trame comme destination

    //type : 2 octets - indique quel protocole est concerné par le message.
    //Ex : 0x0800 (IPv4), 0x0806 (ARP), 0x86DD (IPv6)…
    trame.type[0] = 0x08;
    trame.type[1] = 0x00;

    //DATA (données) : 46 à 1500 octets (y compris bourrage/padding éventuel)
    memset(trame.data, 0, 1500); //Ici on va mettre tout a 0 
    memset(trame.bourrage, 0, 46); 

    //FCS (Frame Check Sequence) : 4 octets - code polynomial détecteur d’erreur
    memset(trame.fcs, 0, 4);

    return trame;
}
    