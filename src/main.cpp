#include <iostream>
#include <vector>
#include "NetworkEvolution/NetworkManager.h"



int main( int argc, char * argv[])
{
    
	srand(time(0));
    NetworkManager test(50,2,2);

    std::vector<std::vector<double>> inputs {{0.2,0.1}};
    std::vector<std::vector<double>> training {{0.1,0.2}};
	
	test.setNetworkInputs(inputs);
	test.setNetworkTraining(training);

    test.reinforcementSimulate(5,100);

	/*
	Genome code(3,3);
	NeuralNetwork skynet(code);
	
	skynet.mutateAddNode(0,3);
	//skynet.mutateAddWeight(6,4);	
	skynet.mutateAddNode(6,4);
	*/
    return 0;
}
