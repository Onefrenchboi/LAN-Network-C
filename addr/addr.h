#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint8_t octet;


typedef struct {
    octet address[4];
} IP;

typedef struct {
    octet address[6];
} MAC;



void print_ip(IP ip);
void print_mac(MAC mac);
void print_mac_sans_double_points(MAC mac);

IP read_ip_from_str(const char* str);
MAC read_mac_from_str(const char* str);

MAC compare_mac(MAC mac1, MAC mac2);

