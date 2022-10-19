# Server Farm Simulator (SFS)

## 1. Definition

SFS is a discrete event simulator written in C++ designed to simulate the behaviour of a large number of servers. It allows to evaluate the performance of different resource orchestration strategies, e.g., reliability, latency, power consumption, etc. SFS also enables the possibility of simulating several scenarios in terms of topologies, guaranteeing high-scalability and heterogeneity. 

## 2. SFS Architecture
The architecture is composed by S servers with M capacity per server. Each server is switched on/off depending on a threshold level (based on the load). 
There are tasks arriving to the system which need to be served by the servers. To handle these tasks, a subset of the $S$ servers is running, while the remaining servers are in one of the available sleep modes to save power, and can be activated on when needed. More specifically, each server can be in one of the following states: (i)~running (i.e., active), a state in which it can serve tasks; (ii)~in a low-power sleep mode; (iii)~switching from a sleep mode to the active mode (or vice versa), when the infrastructure manager decides that more (less) resources are needed; or (iv)~booting-up, after a server crash (we assume that a server can always restart after a crash). The infrastructure manager decides the number of servers that should be active at a given point in time as well as the low-power sleep mode employed. Since there is a clear trade-off between the power consumption and the delay to switch between a low-power state into the active state, selecting the proper sleep state in NFV is crucial for the system performance.

## 3. SFS Features
* The task arrival rate to the system $\lambda$ can be modeled using different random processes. 
* Servers can serve up to M simultaneous tasks and denote by $1/\mu$ the average service time of a task.
* Servers have a finite lifetime, and denote by $1/\beta$ the average lifetime of a server until it crashes, i.e., its mean time to failure (MTTF). 
* We also assume a non-zero boot time with average $1/\alpha$, which is also the recovery time after a crash.

## 4. Folder Structure
    .
    ├── src                               # Source code folder
    │   ├── defines.h                     # File to describe all the variables of the simulator 
    │   ├── functions.cpp                 # File to describe the functionality of all the functions used in the simulator
    │   ├── functions.h                   # File to define the functions
    │   ├── main.cpp                      # File to run the simulations 
    │   └── script.py                     # File to automate a large number of simulations with differents parameters
