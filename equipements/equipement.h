#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "utils.h"
#include "addr.h"
#include "trame.h"


#define SIZE_COMMUTATION_TABLE 128
#define NB_PORTS_MAX 16

typedef struct equipement equipement; //déclarations anticipé de jsp quoi
typedef struct trame_ethernet trame_ethernet;
typedef struct BPDU BPDU;

typedef enum type{
    STATION,
    SWITCH
} type;

typedef struct {
    MAC addr_MAC;
    uint8_t port;
} commutation ;

typedef struct {
    commutation entry[SIZE_COMMUTATION_TABLE];
    int size;
} table_commutation;



typedef struct port {
    equipement* parent;
    uint8_t numero;  
    char status; //R=root port, D=designated port, B=blocked 
    char role; //F=forwarding, L=learning, B=blocking, D=disabled
    struct lien* lien; 
} port;

typedef struct lien {
    port* portA;
    port* portB;
    uint8_t poids; 
} lien;



typedef struct equipement{
    type type;
    MAC addr_MAC;
    port ports[NB_PORTS_MAX]; //tableau de pointeurs vers les ports
    size_t nb_ports; 
} equipement;

typedef struct switch_t{
    equipement base;
    uint16_t priority;
    table_commutation commutation_table;
    BPDU* bpdu; 
} switch_t;

typedef struct station_t{
    equipement base;
    IP addr_IP;
} station_t;




//! functions ici tsais
void print_equipement(equipement* e); //affiche les infos de l'équipement
void print_commutation_table(table_commutation* table); //affihce la table de commutation passée en paramètre
void print_ports(const equipement* e); //affiche les ports de l'équipement
void init_switch(switch_t* sw, char* addr_MAC, uint8_t nb_ports, uint16_t priority); //init switch
void init_station(station_t* sta, char* addr_MAC, char* addr_IP); //init station
void init_commutation_table(table_commutation* table); //init table de commutation

void envoyer_trame(equipement* e, trame_ethernet* trame); //envoie depuis l'equipement e la trame_ethernet trame
void recevoir_trame(equipement* e, trame_ethernet* trame, uint8_t port_numero); //l'equipement e reçoit la trame trame sur le port port_numero
uint8_t existe_dans_commutation_table(table_commutation* table, MAC target); //check si la MAC target existe dans la table de commutation table, retourne le numéro du port si oui, -1 sinon
void update_commutation_table(table_commutation* table, trame_ethernet* trame, uint8_t port_numero); //update la table de commu avec la trame 

bool send_BPDU(equipement* sw);