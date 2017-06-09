#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include "NeuralNetwork/NeuralNetwork.h"
#include <thread>
#include <vector>

class NetworkManager
{
public:
     NetworkManager();
     NetworkManager(unsigned int,unsigned int);
     ~NetworkManager();

     void speciateNetworks();

private:
     std::vector<std::vector<NeuralNetwork*>> networks;
     unsigned int numSpecies;
     unsigned int generationNum;
};

#endif
