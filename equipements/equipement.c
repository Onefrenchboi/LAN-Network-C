#include "equipement.h"
#include <stdio.h>

void print_equipement(equipement* e) {
    if (e->type == STATION) {
        station_t* sta = (station_t*)e;
        printf(MAGENTA("Station\n"));

        printf(BOLDWHITE("  MAC : "));
        print_mac(sta->base.addr_MAC);

        printf(BOLDWHITE("  IP : ")); 
        print_ip(sta->addr_IP); 

    } else if (e->type == SWITCH) {
        switch_t* sw = (switch_t*)e;
        printf(MAGENTA("Switch\n"));

        printf(BOLDWHITE("  MAC : ")); 
        print_mac(sw->base.addr_MAC);

        printf(BOLDWHITE("  Ports : ")); 
        printf(YELLOW("%d\n"), sw->nb_ports);

        printf(BOLDWHITE("  Prio : ")); 
        printf(YELLOW("%d\n"), sw->priority);

    } else {
        printf("t'as tt cassé\n");
    }
}