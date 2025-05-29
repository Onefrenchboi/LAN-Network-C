#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "addr.h"

typedef enum type{
    STATION,
    SWITCH
} type;

typedef struct equipement{
    type type;
    IP addr_IP;
} equipement;

typedef struct switch_t{
    equipement base;
    MAC addr_MAC;
    nb_ports nb_ports;
    uint16_t priority;
    equipement* commutation_table;
} switch_t;

typedef struct station_t{
    equipement base;
} station_t;


//! functions ici tsais
void print_equipement(equipement* e);