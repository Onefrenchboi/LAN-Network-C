#pragma once
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "equipement.h"
#include "addr.h"
#include "utils.h"

typedef struct equipement equipement; //pas sur de comprendre pq on doit redeclarer mais bon


typedef enum type_trame{
    ETHERNET_T,
    BDPU_T
} type_trame;


typedef struct trame_ethernet {
    octet preambule[7];
    octet sfd;
    MAC destination;
    MAC source;
    type_trame type;
    octet data[1500];
    octet bourrage[46];
    octet fcs[4];
} trame_ethernet;

typedef struct BPDU {
    MAC root; //addr du switch root 
    uint8_t cost; //poid pour atteindre le root
    MAC bridge_id; //identification du pont qui envoie le BPDU
} BPDU;


void print_trame_ethernet(const trame_ethernet *trame);
trame_ethernet read_trame_from_str(char* preambule, char* sfd, 
                                    char* destination, char* source, 
                                    char* type, char* data, 
                                    char* bourrage, char* fcs);


trame_ethernet creer_trame_vide(equipement* e, MAC target);
trame_ethernet creer_trame_BPDU(equipement* e);
bool is_BPDU(trame_ethernet* trame);
bool compare_BPDU(BPDU* b1, BPDU* b2);
BPDU read_BPDU_from_trame(trame_ethernet* trame);