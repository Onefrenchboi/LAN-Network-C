#include "addr.h"
#include <stdio.h>
#include "utils.h"


void print_ip(IP ip) {
    printf(YELLOW("%u.%u.%u.%u"), ip.address[0], ip.address[1], ip.address[2], ip.address[3]);
}

void print_mac(MAC mac) {
    printf(YELLOW("%02x:%02x:%02x:%02x:%02x:%02x"),
           mac.address[0], mac.address[1], mac.address[2],
           mac.address[3], mac.address[4], mac.address[5]);
}



IP read_ip_from_str(const char* str) {
    IP ip;
    sscanf(str, "%hhu.%hhu.%hhu.%hhu",
           &ip.address[0], &ip.address[1], &ip.address[2],
           &ip.address[3]);
    return ip;
}

MAC read_mac_from_str(const char* str) {
    MAC mac;
    sscanf(str, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
           &mac.address[0], &mac.address[1], &mac.address[2],
           &mac.address[3], &mac.address[4], &mac.address[5]);
    return mac;
}