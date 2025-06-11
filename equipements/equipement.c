#include "equipement.h"
#include <stdio.h>

void init_switch(switch_t* sw, char* addr_MAC, uint8_t nb_ports, uint16_t priority) {
    sw->base.type = SWITCH;
    sw->base.addr_MAC = read_mac_from_str(addr_MAC);
    sw->base.nb_ports = nb_ports;
    sw->priority = priority;
    init_commutation_table(&sw->commutation_table);

    for (size_t i = 0; i < nb_ports; i++) {
        sw->base.ports[i].parent = (equipement*)sw;
        sw->base.ports[i].numero = i;
        sw->base.ports[i].lien = NULL;
    }
}

void init_station(station_t* sta, char* addr_MAC, char* addr_IP) {
    sta->base.type = STATION;
    sta->base.addr_MAC = read_mac_from_str(addr_MAC);
    sta->base.nb_ports = 1;
    sta->addr_IP = read_ip_from_str(addr_IP);

    sta->base.ports[0].parent = (equipement*)sta;
    sta->base.ports[0].numero = 0;
    sta->base.ports[0].lien = NULL;
}

void init_commutation_table(table_commutation* table){
    table->size = 0;
}



void envoyer_broadcast(equipement* e, trame_ethernet* trame, uint8_t port_exclu) {
    for (uint8_t i = 0; i < e->nb_ports; ++i) {
        if (i == port_exclu) {
            continue;
        }
        port* p = &e->ports[i];
        if (p->lien != NULL) {
            port* voisin_port = (p->lien->portA == p) ? p->lien->portB : p->lien->portA;
            equipement* voisin = voisin_port->parent;
            printf(CYAN("Envoi d'une trame de "));
            print_mac(e->addr_MAC);
            printf(CYAN(" à "));
            print_mac(voisin->addr_MAC);
            printf(CYAN(" via le port %d\n"), p->numero);
            printf("--------------------------------------\n");
            recevoir_trame(voisin, trame, voisin_port->numero);
        }
    }
}




void envoyer_trame(equipement* e, trame_ethernet* trame) {
    MAC target = trame->destination;
    if (memcmp(&target, &trame->source, sizeof(MAC)) == 0) {
        printf("Ca va la schizophrénie ? tu tparles a toi-même\n");
        return;
    }
    
    //Si c'est une station, on envoie direct vu qu'elle est connectée qu'a un seul truc (un switch)
    if (e->type == STATION) {
        port* p = &e->ports[0];
        port* voisin_port = (p->lien->portA == p) ? p->lien->portB : p->lien->portA;
        equipement* voisin = voisin_port->parent;
        printf(CYAN("Envoi d'une trame de "));
        print_mac(e->addr_MAC);
        printf(CYAN(" à "));
        print_mac(voisin->addr_MAC);
        printf(CYAN(" via le port %d\n"), p->numero);
        printf("--------------------------------------\n");
        recevoir_trame(voisin, trame, voisin_port->numero);
        return;
    }
    
    switch_t* s = (switch_t*)e;
    uint8_t port_dest = existe_dans_commutation_table(&s->commutation_table, trame->destination);

    if (port_dest != (uint8_t)-1) { //techniquement le cast en int va faire que -1 c'est 255 mais chuuuuut on a def le nb max de port a 16
        port* p = &e->ports[port_dest];
        port* voisin_port = (p->lien->portA == p) ? p->lien->portB : p->lien->portA; //mueeheheh ternaire
        equipement* voisin = voisin_port->parent;
        printf("--------------------------------------\n");
        printf(CYAN("Switch envoie unicast : "));
        print_mac(e->addr_MAC);
        printf(CYAN(" à "));
        print_mac(voisin->addr_MAC);
        printf(CYAN(" via le port %d\n"), p->numero);
        printf("--------------------------------------\n");

        
        recevoir_trame(voisin, trame, p->numero);
    } else {
        printf("La MAC ");
        print_mac(target);
        printf(" n'est pas dans la table de commutation, envoi en broadcast.\n");
        printf("--------------------------------------\n");

        envoyer_broadcast(e, trame,-1);
    }
}





void recevoir_trame(equipement* e, trame_ethernet* trame, uint8_t port_numero){
    if (e->type==STATION) {
        printf(MAGENTA("\nTrame reçue sur la station "));
        print_ip(((station_t*)e)->addr_IP);


        printf(MAGENTA(" (MAC : "));
        print_mac(e->addr_MAC);
        printf(MAGENTA(")\n"));

        printf(MAGENTA("\nTrame reçue sur le port %d\n"), port_numero);
        printf(MAGENTA("Trame envoyée par : "));
        print_mac(trame->source);
        printf("\n--------------------------------------\n");
        return;
    }

    switch_t* s = (switch_t*)e;

    update_commutation_table(&s->commutation_table, trame, port_numero);

    printf(MAGENTA("Trame reçue sur le switch : "));
    print_mac(((switch_t*)e)->base.addr_MAC);
    printf(MAGENTA("\nTrame reçue sur le port %d\n"), port_numero);
    printf(MAGENTA("Trame envoyée par : "));
    print_mac(trame->source);
    printf("\n--------------------------------------\n");

    uint8_t port_dest = existe_dans_commutation_table(&s->commutation_table, trame->destination);
    if (port_dest != (uint8_t)-1 && port_dest != port_numero) {
        port* p = &e->ports[port_dest];
        if (p->lien) {
            port* voisin_port = (p->lien->portA == p) ? p->lien->portB : p->lien->portA;
            equipement* voisin = voisin_port->parent;
            printf(MAGENTA("Switch relaye unicast: "));
            print_mac(e->addr_MAC);
            printf(MAGENTA(" à "));
            print_mac(voisin->addr_MAC);
            printf(MAGENTA(" via port %d\n"), p->numero);
            recevoir_trame(voisin, trame, voisin_port->numero);
        }
    } else {
        printf(MAGENTA("Switch flooding sur autres ports (hors port arrivée)\n"));
        envoyer_broadcast(e, trame, port_numero);
    }
    
}

//return le num du port si la MAC existe dans la table de commutation, -1 sinon
uint8_t existe_dans_commutation_table(table_commutation* table, MAC target){
    for (int i = 0; i < table->size; ++i) {
        if (memcmp(&table->entry[i].addr_MAC, &target, sizeof(MAC)) == 0) {
            return table->entry[i].port;
        }
    }
    return (uint8_t)-1;
}

void update_commutation_table(table_commutation* table, trame_ethernet* trame, uint8_t port_numero) {
    for (int i = 0; i < table->size; ++i) {
        if (memcmp(&table->entry[i].addr_MAC, &trame->source, sizeof(MAC)) == 0) {
            table->entry[i].port = port_numero;
            return;
        }
    }
    table->entry[table->size].addr_MAC = trame->source;
    table->entry[table->size].port = port_numero;
    table->size++;

}

void print_equipement(equipement* e) {
    if (e->type == STATION) {
        station_t* sta = (station_t*)e;
        printf(MAGENTA("Station\n"));

        printf(BOLDWHITE("  MAC : "));
        print_mac(sta->base.addr_MAC);

        printf(BOLDWHITE("\n  IP : ")); 
        print_ip(sta->addr_IP); 

    } else if (e->type == SWITCH) {
        switch_t* sw = (switch_t*)e;
        printf(MAGENTA("Switch\n"));

        printf(BOLDWHITE("  MAC : ")); 
        print_mac(sw->base.addr_MAC);

        printf(BOLDWHITE("  Ports : ")); 
        printf(YELLOW("%zu\n"), sw->base.nb_ports);

        printf(BOLDWHITE("  Prio : ")); 
        printf(YELLOW("%d\n"), sw->priority);
;
        print_commutation_table(&sw->commutation_table);
    } else {
        printf("t'as tt cassé\n");
    }
}
void print_commutation_table(table_commutation* table) {
    printf(BOLDWHITE("Table de commutation du switch :\n"));
    printf("┌─────────────────────────────┬────────┐\n");
    printf("│        Adresse MAC          │  Port  │\n");
    printf("├─────────────────────────────┼────────┤\n");

    for (int i = 0; i < table->size; ++i) {
        printf("│      ");
        print_mac(table->entry[i].addr_MAC);
        printf("      │   %2d   │\n", table->entry[i].port);
    }

    printf("└─────────────────────────────┴────────┘\n");
}