#include "equipements/equipement.h"
#include "graphe/graphe.h"
#include "graphe/algos.h"
#include "reseau/reseau.h"
#include "trame/trame.h"


int main() {

    station_t* sta = malloc(sizeof(station_t));
    sta->base.type = STATION;
    IP ip = {{192, 168, 1, 10}};
    sta->base.addr_IP = ip;


    switch_t* sw = malloc(sizeof(switch_t));
    sw->base.type = SWITCH;
    IP ip2 = {{192, 168, 1, 1}};
    sw->base.addr_IP = ip2;
    MAC mac ={{0x00, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE}};
    sw->addr_MAC = mac;
    sw->nb_ports = 8;
    sw->priority = 32768;


    print_equipement((equipement*)sta);
    printf("\n");
    print_equipement((equipement*)sw);

    free(sta);
    free(sw);

    return 0;
}