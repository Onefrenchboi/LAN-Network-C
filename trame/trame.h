#pragma once
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "equipement.h"
#include "addr.h"
#include "utils.h"

typedef struct equipement equipement; //pas sur de comprendre pq on doit redeclarer mais bon

typedef struct trame_ethernet {
    octet preambule[7];
    octet sfd;
    MAC destination;
    MAC source;
    octet type[2];
    octet data[1500];
    octet bourrage[46];
    octet fcs[4];
} trame_ethernet;





typedef struct BDPU {g

} BDPU;


void print_trame_ethernet(const trame_ethernet *trame);
trame_ethernet read_trame_from_str(char* preambule, char* sfd, 
                                    char* destination, char* source, 
                                    char* type, char* data, 
                                    char* bourrage, char* fcs);


trame_ethernet creer_trame_vide(equipement* e, MAC target);