#include <iostream>
#include <vector>
#include "NetworkEvolution/NetworkManager.h"



int main( int argc, char * argv[])
{

    //This is a test genome that I have wrote to a file for testing.
    Genome code(2,50);
    NeuralNetwork testNetwork(code);
    testNetwork.visualize(900,600);
    return 0;
}
