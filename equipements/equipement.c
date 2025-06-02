#include "equipement.h"
#include <stdio.h>

void print_equipement(equipement* e) {
    if (e->type == STATION) {
        station_t* sta = (station_t*)e;
        printf("Station\n");
        printf("  MAC : "); print_mac(sta->base.addr_MAC); printf("\n");
        printf("  IP : "); print_ip(sta->addr_IP); printf("\n");
    } else if (e->type == SWITCH) {
        switch_t* sw = (switch_t*)e;
        printf("Switch\n");
        printf("  MAC : "); print_mac(sw->base.addr_MAC); printf("\n");
        printf("  Ports : %d\n", sw->nb_ports);
        printf("  Prio : %d\n", sw->priority);
    } else {
        printf("t'as tt cassé\n");
    }
}