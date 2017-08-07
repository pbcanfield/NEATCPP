#include <iostream>
#include <vector>
#include "NetworkEvolution/NetworkManager.h"



int main( int argc, char * argv[])
{

    NetworkManager NEAT(100,2,4);


    std::vector<std::vector<double>> inputs{{0.1,0.1},{0.2,0.2}};
    std::vector<std::vector<double>> lables{{0.99,0.4,0.1,0.5},{0.1,0.7,0.3,0.3}};

    NEAT.trainNetworksOnline(inputs,lables,10);



    return 0;

}
