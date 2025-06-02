#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "equipement.h"



typedef struct reseau{
    int nb_equipements;
    int nb_liens;
    equipement** equipements; //ptr de ptr parce qu'on va prendre un ptr sur chacune des trucs pour toucher dessus
} reseau;




void read_conf(char* filename);