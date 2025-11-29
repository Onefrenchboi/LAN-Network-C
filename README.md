# Simulation of a LAN network in C



[![fr](https://img.shields.io/badge/lang-fr-blue.svg)](./README.fr.md)


## About the project

**This project was conducted during my first year of Computer Science at IUT Robert Schuman, under teacher supervision and in an academic context.**


We want to develop a simulation of **a LAN network**, in C. The main goal is to be able to create *computers* and *switches*, with their own addresses, and have them send messages to eachother.

## Organization

- **Group work** with [Matéo W.](https://github.com/Zeyn-n) and [Mathis D.]()

### Technologies Used

* **C**: Main programming language
* **Generic pointers**: Use of casting to create a flexible and modular network simulation
* **Makefile**: Compilation via enriched [Makefile](./Makefile)

## How to Run the Project


### Prerequisites

- **GCC** installed on your machine

### Installation

1. Clone the repository to your local machine:
    ```bash
    git clone https://github.com/Onefrenchboi/LAN-Network-C.git
    ```

### Manual Compilation and Execution

```bash
# Compile the project
make

# Run the network
./bin/network
```

## Main Features

* **Structures**: Special structures to represent the different parts of a network
  * **Switches and Machines**: Create and manage network devices with unique addresses
* **Sending**: Send "pings" to other machines in the network
* **Display**: Visualize network frames as they are exchanged
* **Spanning Tree Protocol (STP)**: Simulate STP to prevent network loops
* **Configuration File Support**: Load network from a configuration file