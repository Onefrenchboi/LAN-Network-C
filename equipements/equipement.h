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
    IP addr_IP;
    type type;
    
} equipement;

typedef struct switch_t{
    IP addr_IP;
    type type;
    MAC addr_MAC;
    nb_ports nb_ports;
    uint16_t priority;
    equipement* commutation_table;
} switch_t;

typedef struct station_t{
    IP addr_IP;
    type type;

} station_t;


//! functions ici tsais