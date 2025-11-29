
# Simulation d’un réseau LAN en C

[![en](https://img.shields.io/badge/lang-en-red.svg)](./README.md)

## À propos du projet

**Ce projet a été réalisé lors de ma première année d’informatique à l’IUT Robert Schuman, sous la supervision d’un enseignant et dans un cadre académique.**

Nous souhaitons développer une simulation d’**un réseau LAN** en C. L’objectif principal est de pouvoir créer des *ordinateurs* et des *switchs*, avec leurs propres adresses, et de leur permettre d’envoyer des messages entre eux.

## Organisation

- **Travail de groupe** avec [Matéo W.](https://github.com/Zeyn-n) et [Mathis D.](https://github.com/KitsuneRandom/KitsuneRandom)

### Technologies utilisées

* **C** : Langage de programmation principal
* **Pointeurs génériques** : Utilisation du cast pour créer une simulation de réseau flexible et modulaire
* **Makefile** : Compilation via un [Makefile](./Makefile) enrichi

## Comment exécuter le projet

### Prérequis

- **GCC** installé sur votre machine

### Installation

1. Clonez le dépôt sur votre machine locale :
    ```bash
    git clone https://github.com/Onefrenchboi/LAN-Network-C.git
    ```

### Compilation et exécution manuelles

```bash
# Compiler le projet
make

# Lancer le réseau
./bin/network
```

## Fonctionnalités principales

* **Structures** : Structures spécifiques pour représenter les différentes parties d’un réseau
  * **Commutateurs et machines** : Créez et gérez des équipements réseau avec des adresses uniques
* **Envoi** : Envoyez des « pings » à d’autres machines du réseau
* **Affichage** : Visualisez les trames réseau échangées
* **Spanning Tree Protocol (STP)** : Simulez le STP pour éviter les boucles réseau
* **Support de fichier de configuration** : Chargez le réseau à partir d’un fichier de configuration