#include "addr.h"


void print_ip(IP ip) {
    printf("%u.%u.%u.%u", ip.address[0], ip.address[1], ip.address[2], ip.address[3]);
}

void print_mac(MAC mac) {
    printf("%02x:%02x:%02x:%02x:%02x:%02x",
           mac.address[0], mac.address[1], mac.address[2],
           mac.address[3], mac.address[4], mac.address[5]);
}