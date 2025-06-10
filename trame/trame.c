#include "trame.h"
#include <stdio.h>

void print_octets(const octet *tab, size_t taille) {
    printf("%02x", tab[0]);
    for (size_t i = 1; i < taille; ++i) {
        printf(":%02x", tab[i]);
    }
    printf("\n");
}

void print_trame_ethernet(const trame_ethernet *t) {
    if (t == NULL) {
        printf("J'ai dit une trame pas un tram\n");
        return;
    }
    printf("Trame :\n");
    printf("Préambule : ");
    print_octets(t->preambule, sizeof(t->preambule));
    printf("SFD : %02x\n", t->sfd);
    printf("Destination : ");
    print_octets(t->destination, sizeof(t->destination));
    printf("Source : ");
    print_octets(t->source, sizeof(t->source));
    printf("Type : ");
    print_octets(t->type, sizeof(t->type));
    printf("Données : ");
    print_octets(t->data, sizeof(t->data));
    printf("Bourrage : ");
    print_octets(t->bourrage, sizeof(t->bourrage));
    printf("FCS : ");
    print_octets(t->fcs, sizeof(t->fcs));
    printf("\n");
}

trame_ethernet read_trame_from_str(char* preambule, char* sfd, 
                                    char* destination, char* source, 
                                    char* type, char* data, 
                                    char* bourrage, char* fcs) {
    trame_ethernet trame;
    sscanf(preambule, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
           &trame.preambule[0], &trame.preambule[1], &trame.preambule[2],
           &trame.preambule[3], &trame.preambule[4], &trame.preambule[5],
           &trame.preambule[6]);
    sscanf(sfd, "%02hhx", &trame.sfd);
    sscanf(destination, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
           &trame.destination[0], &trame.destination[1], &trame.destination[2],
           &trame.destination[3], &trame.destination[4], &trame.destination[5]);
    sscanf(source, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
           &trame.source[0], &trame.source[1], &trame.source[2],
           &trame.source[3], &trame.source[4], &trame.source[5]);
    sscanf(type, "%02hhx:%02hhx", &trame.type[0], &trame.type[1]);
    for (size_t i = 0; i < sizeof(trame.data); ++i) {
        sscanf(data + i * 3, "%02hhx", &trame.data[i]);
    }
    for (size_t i = 0; i < sizeof(trame.bourrage); ++i) {
        sscanf(bourrage + i * 3, "%02hhx", &trame.bourrage[i]);
    }
    sscanf(fcs, "%02hhx:%02hhx:%02hhx:%02hhx", &trame.fcs[0], &trame.fcs[1], &trame.fcs[2], &trame.fcs[3]);
    return trame;
}