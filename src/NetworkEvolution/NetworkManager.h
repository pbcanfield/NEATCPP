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
	
    //Required setup functions for reinforcment learning
	void setNetworkInputs(std::vector<std::vector<double>>);
	void setNetworkTraining(std::vector<std::vector<double>>);
	
	//Optional setup functions
	void setMutationProbability(float, float, float, float); 	
	//Supervised learning functions.
    void trainNetworksOnline(unsigned int, double=0.01);

    void sortSupervisedNetworks();

    //Genetic/Reinforcment learning functions.

	void reinforcementSimulate(unsigned int,unsigned int, double=0.01);

    //Evolution functions.
    void crossTopHalf();

private:
	void quicksortVector(std::vector<NeuralNetwork*> &, int,int);
    bool isMutation();
	
	std::vector<std::vector<double>> inputData;
	std::vector<std::vector<double>> trainingData;

	std::vector<std::vector<NeuralNetwork*>> networks;
	float mutationProb = 0.1;
	float nodeAddProb = 0.3;
	float biasAddProb = 0.1;
	float connectionProb = 0.6;

};

#endif
