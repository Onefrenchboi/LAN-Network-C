#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "utils.h"
#include "addr.h"
#include "trame.h"

typedef enum type{
    STATION,
    SWITCH
} type;

typedef struct equipement{
    type type;
    MAC addr_MAC;
} equipement;

typedef struct switch_t{
    equipement base;
    nb_ports nb_ports;
    uint16_t priority;
    equipement* commutation_table;
} switch_t;

typedef struct station_t{
    equipement base;
    IP addr_IP;
} station_t;


//! functions ici tsais
void print_equipement(equipement* e);