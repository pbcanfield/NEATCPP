#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include "NeuralNetwork/NeuralNetwork.h"
#include <thread>
#include <vector>

class NetworkManager
{
public:
    NetworkManager();
    NetworkManager(unsigned int,unsigned int, unsigned int);
    ~NetworkManager();


private:
    std::vector<std::vector<NeuralNetwork*>> networks;
};

#endif
