#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "addr.h"

typedef struct trame_ethernet {
    octet preambule[7];
    octet sfd;
    octet destination[6];
    octet source[6];
    octet type[2];
    octet data[1500];
    octet bourrage[46];
    octet fcs[4];
} trame_ethernet;

void print_trame_ethernet(const trame_ethernet *trame);
trame_ethernet read_trame_from_str(char* preambule, char* sfd, 
                                    char* destination, char* source, 
                                    char* type, char* data, 
                                    char* bourrage, char* fcs);