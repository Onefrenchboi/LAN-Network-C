#include "equipement.h"
#include <stdio.h>


//Initialisations 
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
        sw->base.ports[i].status = 'F'; //forwarding par défaut
        sw->base.ports[i].role = 'U'; //U pour unknown parce qu'on sait pas encore

    //Initialisation de la BPDU
    sw->bpdu = malloc(sizeof(BPDU));
    if (sw->bpdu == NULL) {
        perror("Erreur d'allocation mémoire pour la BPDU");
        exit(EXIT_FAILURE);
    }
    sw->bpdu->root = sw->base.addr_MAC; //adresse mac du switch
    sw->bpdu->cost = 0; //0 de cost
    sw->bpdu->bridge_id = sw->base.addr_MAC; //aussii l'addr mac
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






//Envoi de trames
void envoyer_broadcast(equipement* e, trame_ethernet* trame, uint8_t port_exclu) {
    for (uint8_t i = 0; i < e->nb_ports; ++i) {

        if (i == port_exclu) {
            continue;
        }
        port* p = &e->ports[i];
        if (p->status == 'B') {
            continue;
        }
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
    printf(BOLDGREEN("\nEnvoi d'une trame de "));
    print_mac(e->addr_MAC);
    printf(BOLDGREEN(" à "));
    print_mac(target);
    printf(BOLDGREEN("\n------------------------------------------------------------------------\n"));


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
        printf(CYAN(" via le port "));
        printf(GREEN("%d\n"), p->numero);
        printf("--------------------------------------\n");
        recevoir_trame(voisin, trame, voisin_port->numero);
        return;
    }

    //Si c'est pas une station, c'est un switch
    switch_t* s = (switch_t*)e;
    uint8_t port_dest = existe_dans_commutation_table(&s->commutation_table, trame->destination);

    if (port_dest != (uint8_t)-1) { //techniquement le cast en int va faire que -1 c'est 255 mais chuuuuut on a def le nb max de port a 16
        
        port* p = &e->ports[port_dest];
        if (p->status=='B') {
            return;
        }

        //on envoie la trame au port correspondant
        port* voisin_port = (p->lien->portA == p) ? p->lien->portB : p->lien->portA; //mueeheheh ternaire
        equipement* voisin = voisin_port->parent;
        printf("--------------------------------------\n");
        printf(CYAN("Switch envoie unicast : "));
        print_mac(e->addr_MAC);
        printf(CYAN(" à "));
        print_mac(voisin->addr_MAC);
        printf(CYAN(" via le port "));
        printf(GREEN("%d\n"), p->numero);
        printf("--------------------------------------\n");


        recevoir_trame(voisin, trame, p->numero);
    } 
    else {
        printf("La MAC ");
        print_mac(target);
        printf(" n'est pas dans la table de commutation, envoi en broadcast.\n");
        printf("--------------------------------------\n");

        envoyer_broadcast(e, trame, -1);
    }
}

//Reception de trames
void recevoir_trame(equipement* e, trame_ethernet* trame, uint8_t port_numero){

    if (e->type==STATION) {
        printf(MAGENTA("Trame reçue sur la station "));
        print_ip(((station_t*)e)->addr_IP);

        printf(MAGENTA(" (MAC : "));
        print_mac(e->addr_MAC);
        printf(MAGENTA(" )\n"));

        printf(MAGENTA("Trame reçue sur le port %d\n"), port_numero);
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


    //Si c'est un BPDU, on le traite
    if (is_BPDU(trame)) {
    BPDU b = read_BPDU_from_trame(trame);
    printf(MAGENTA("BPDU reçu :\n"));
    printf(BOLDWHITE("  Root : "));
    print_mac(b.root);
    printf(BOLDWHITE("\n  Cost : %d\n"), b.cost);
    printf(BOLDWHITE("  Bridge ID : "));
    print_mac(b.bridge_id);
    printf("\n");

    if (compare_BPDU(&b, s->bpdu)) { 
        printf(MAGENTA("Le BPDU reçu est meilleur que le BPDU actuel, on met à jour.\n"));
        s->bpdu->root = b.root;
        s->bpdu->cost = b.cost + s->bpdu->cost;


        for (size_t i = 0; i < e->nb_ports; ++i) {
            port* p = &e->ports[i];
            if (i == port_numero) {
                p->role = 'R';
                p->status = 'F';
            } else if (p->role != 'D') {
                p->role = 'B';
                p->status = 'B';
            }
        }
    } else {
        printf(MAGENTA("Le BPDU reçu n'est pas meilleur que le BPDU actuel, on ne fait rien.\n"));
    }
    return;
}

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
        printf("--------------------------------------\n");
        envoyer_broadcast(e, trame, port_numero);
    }
}




bool send_BPDU(equipement* e) {
    if (e->type != SWITCH) {
        printf(RED("L'équipement n'est pas un switch, pas de BPDU envoyé.\n"));
        return false;
    }
    switch_t* sw = (switch_t*)e;

    printf(MAGENTA("Envoi de BPDU par "));
    print_mac(sw->base.addr_MAC);
    printf("\n");
    printf(BOLDWHITE("  Root : "));
    print_mac(sw->bpdu->root);
    printf(BOLDWHITE("\n  Cost : %d\n"), sw->bpdu->cost);
    printf(BOLDWHITE("  Bridge ID : "));

    bool bpdu_sent = false;
    for (size_t i = 0; i < sw->base.nb_ports; i++) {
        port* p = &sw->base.ports[i];
        if (p->status == 'F'){
            trame_ethernet bpdu_trame = creer_trame_BPDU((equipement*)sw);
            envoyer_trame((equipement*)sw, &bpdu_trame);
            bpdu_sent = true;
        }
    }
    return bpdu_sent;
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







//update la table de commutation avec la trame reçue et le port sur lequel elle a été reçue
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

void print_ports(const equipement* e) {
    if (e->type != SWITCH) {
        printf("Pas un switch.\n");
        return;
    }
    const switch_t* sw = (const switch_t*)e;
    printf(BOLDWHITE("État des ports du switch :\n"));
    printf("┌───────┬────────┬──────┐\n");
    printf("│ Port  │ State  │ Role │\n");
    printf("├───────┼────────┼──────┤\n");
    for (size_t i = 0; i < sw->base.nb_ports; ++i) {
        // Couleur pour l'état
        char* state_color = "";
        switch (sw->base.ports[i].status) {
            case 'F': state_color = "\033[1;32m"; break; // Vert
            case 'B': state_color = "\033[1;31m"; break; // Rouge
            case 'L': state_color = "\033[1;33m"; break; // Jaune
            default:  state_color = "\033[0m";    break; // Reset
        }
        // Couleur pour le rôle
        char* role_color = "";
        switch (sw->base.ports[i].role) {
            case 'R': role_color = "\033[1;36m"; break; // Cyan
            case 'D': role_color = "\033[1;34m"; break; // Bleu
            case 'B': role_color = "\033[1;31m"; break; // Rouge
            default:  role_color = "\033[0m";    break; // Reset
        }
        printf("│  %2zu   │   %s%c\033[0m    │  %s%c\033[0m   │\n",
               i,
               state_color, sw->base.ports[i].status,
               role_color, sw->base.ports[i].role);
    }
    printf("└───────┴────────┴──────┘\n");
}