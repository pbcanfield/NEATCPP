#include <iostream>
#include <vector>
#include "NetworkEvolution/NetworkManager.h"



int main( int argc, char * argv[])
{
    
	srand(time(0));
    
	
	/*
	NetworkManager test(200,2,2);

    std::vector<std::vector<double>> inputs {{0.2,0.1}};
    std::vector<std::vector<double>> training {{0.1,0.2}};
	
	test.setNetworkInputs(inputs);
	test.setNetworkTraining(training);

    test.reinforcementSimulate(100,10);

	
	Genome code(3,3);
	NeuralNetwork skynet(code);
	
	skynet.mutateAddNode(0,3);
	//skynet.mutateAddWeight(6,4);	
	skynet.mutateAddNode(6,4);
	*/
	

	NeuralNetwork * netOne = new NeuralNetwork(Genome(2,2));
	NeuralNetwork * netTwo = new NeuralNetwork(Genome(2,2));

	netOne -> mutateAddNode(0,2);

	NeuralNetwork * netThree;
	netThree = *netOne + netTwo;

	//netOne -> visualize(900,600);
	//netTwo -> visualize(900,600);
	//netThree -> visualize(900,600);

	delete netOne;
	delete netTwo;
	delete netThree;
	
    return 0;
}
