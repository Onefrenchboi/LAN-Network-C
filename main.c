#include "equipements/equipement.h"
#include "graphe/graphe.h"
#include "graphe/algos.h"
#include "reseau/reseau.h"
#include "trame/trame.h"
#include <stdio.h>



int main() {


    char* ip = "192.168.24.1";
    char* mac1 = "01:45:23:a6:f7:01";
    char* mac2 = "54:d6:a6:82:c5:08";
    station_t* sta = malloc(sizeof(station_t));
    sta->base.type = STATION;
    sta->addr_IP = read_ip_from_str(ip);
    sta->base.addr_MAC = read_mac_from_str(mac2);


    switch_t* sw = malloc(sizeof(switch_t));
    sw->base.type = SWITCH;
    sw->base.addr_MAC = read_mac_from_str(mac1);
    sw->nb_ports = 8;
    sw->priority = 32768;


    print_equipement((equipement*)sta);
    printf("\n");
    print_equipement((equipement*)sw);

    free(sta);
    free(sw);

    return 0;
}


