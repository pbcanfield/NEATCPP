#include <iostream>
#include <vector>
#include "NetworkEvolution/NetworkManager.h"



int main( int argc, char * argv[])
{
    srand(time(0));
    NetworkManager test(200,2,2);

    std::vector<std::vector<double>> inputs {{0.2,0.1}};
    std::vector<std::vector<double>> training {{0.1,0.2}};


    test.trainNetworksOnline(inputs,training,1000);

    test.sortSupervisedNetworks();

    return 0;
}
