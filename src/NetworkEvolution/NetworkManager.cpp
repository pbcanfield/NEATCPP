#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
     generationNum = 0;
     numSpecies = 0;
}

NetworkManager::NetworkManager(unsigned int startingNum, unsigned int numSpecies)
{
     this -> numSpecies = numSpecies;

}

NetworkManager::~NetworkManager()
{
     for(auto & vec : networks)
          for(auto & net : vec)
               delete net;
}

void NetworkManager::speciateNetworks()
{

}
