#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include "NeuralNetwork/NeuralNetwork.h"
#include <thread>
#include <vector>

class NetworkManager
{
public:
    NetworkManager();
    NetworkManager(unsigned int, unsigned int, unsigned int);
    ~NetworkManager();

    //Supervised learning functions.
    void trainNetworksOnline(std::vector<std::vector<double>>,
                    std::vector<std::vector<double>>,
                    unsigned int, double=0.01);

    void sortSupervisedNetworks();

    //Genetic/Reinforcment learning functions.

    //Evolution functions.
    void crossTopHalf();

private:

    void quicksortVector(std::vector<NeuralNetwork*>&, int, int);
    std::vector<std::vector<NeuralNetwork*>> networks;

};

#endif
