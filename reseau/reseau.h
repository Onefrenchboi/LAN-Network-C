#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "equipement.h"

typedef struct lien
{
  equipement* s1;
  equipement* s2;
  uint8_t poids;
} lien;


typedef struct reseau{
    size_t nb_equipements;
    equipement** equipements; //ptr de ptr parce qu'on va prendre un ptr sur chacune des trucs pour toucher dessus
    lien* liens;
    size_t nb_liens;
} reseau;




void read_conf(char* filename, reseau* reseau);
void afficher_reseau(reseau* reseau);
void deinit_reseau(reseau* reseau);