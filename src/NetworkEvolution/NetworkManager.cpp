#include "NetworkManager.h"

NetworkManager::NetworkManager()
{

}

NetworkManager::NetworkManager(unsigned int populationSize, unsigned int input,
                               unsigned int output)
{
    Genome startingNetwork(input,output);
    std::vector<NeuralNetwork*> nets;
    for (unsigned int i = 0; i < populationSize; ++i)
        nets.push_back(new NeuralNetwork(startingNetwork));

    networks.push_back(nets);
}

NetworkManager::~NetworkManager()
{
     for(auto & vec : networks)
          for(auto & net : vec)
               delete net;
}
