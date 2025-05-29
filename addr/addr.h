#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint8_t octet;
typedef uint8_t nb_ports;


typedef struct {
    octet address[4];
} IP;

typedef struct {
    octet address[6];
} MAC;


//?Fonctions

void print_ip(IP ip);
void print_mac(MAC mac);

