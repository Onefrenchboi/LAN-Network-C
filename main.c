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




    trame_ethernet* t = malloc(sizeof(trame_ethernet));
    for (int i = 0; i < 7; i++) {
        t->preambule[i] = 0b10101010;
    }
    t->sfd = 1;
    for (int i = 0; i < 6; i++) {
        t->destination[i] = sta->base.addr_MAC.address[i];
        t->source[i] = sw->base.addr_MAC.address[i];
    }
    t->type[0] = 0x08;
    t->type[1] = 0x00;
    for (int i = 0; i < 1500; i++) {
        t->data[i] = i;
    }
    for (int i = 0; i < 46; i++) {
        t->bourrage[i] = 0;
    }
    for (int i = 0; i < 4; i++) {
        t->fcs[i] = 0;
    }trame_ethernet* t2 = malloc(sizeof(trame_ethernet));
    *t2 = read_trame_from_str("aa:aa:aa:aa:aa:aa:aa", "01", 
        "01:45:23:a6:f7:01", "54:d6:a6:82:c5:08", 
        "08:00", 
        "01:02:03:04:05:06:07:08:09:0a:0b:0c:0d:0e:0f", 
        "00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00", 
        "00,00");

    // print_equipement((equipement*)sta);
    // printf("\n");
    // print_equipement((equipement*)sw);
    // printf("\n");
    // print_trame_ethernet(t);
    // print_trame_ethernet(t2);

    free(sta);
    free(sw);
    free(t);

    reseau r;
    read_conf("test.txt", &r);
    afficher_reseau(&r);
    deinit_reseau(&r);

    return 0;
}


