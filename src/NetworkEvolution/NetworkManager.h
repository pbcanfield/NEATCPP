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
	
	/**
	 * Sets the inputs that will be used for each NeuralNetwork.
	 * @param std::vector<std::vector<double>>  This is the inputs that are used
	 *                                          for each NeualNetwork, each 
	 *                                          repetition of the input data must
	 *                                          be the size of the inputs.
	 */
	void setNetworkInputs(std::vector<std::vector<double>>);
	
	`/**
	 * Sets the training values that will be used for each NeuralNetwork.
	 * @param std::vector<std::vector<double>>  This is the trainging values that are used
	 *                                          for each NeualNetwork, each 
	 *                                          repetition of the training data must
	 *                                          be the size of the inputs.
	 */
	void setNetworkTraining(std::vector<std::vector<double>>);
	
	//Optional setup functions
	
	/**
	 * Sets the probablilities that will be used to determine the mutation rates.
	 * @param float  This is the probablility that a mutation will occur at all.
	 * @param float  This is the probablility that a node mutation will occur if a mutation
	 *               occurs in the first place.
	 * @param float  This is the probability that a connection mutation will occur if a 
	 *               mutation occurs.
	 * @param float  This is the probablity that a bias mutation will occur if a 
	 *               mutation occurs.
	 */
	void setMutationProbability(float, float, float, float); 	
	
	
	//Supervised learning functions.

	/**
	 * This function iterates through every network in the population, assigns the 
	 * training data to each NeuralNetwork, then through the data an epochs number of times
	 * at a learning rate that can be specified but defualts to 0.01.
	 * @param unsigned int  This is the number of epochs that the NueralNetworks will be trained on.
	 * @param double        This is the learning rate that will be used for each NeuralNetwork.
	 */
	void trainNetworksOnline(unsigned int, double=0.01);


	/**
	 * This is a function that uses the quicksort algorithm to sort every NeuralNetwork by LMSError 
	 * from the current best performance to the worst performing network.
	 */
    void sortSupervisedNetworks();

    //Genetic/Reinforcment learning functions.

	/**
	 * This is a function that the Nueral networks and allows the training process where the
	 * NeuralNetworks are repeatedly trained, compared, and crossed with the trainNetworksOnline,
	 * sortSupervisedNetworks, and crosstopHalf functions respectively.
	 * @param unsigned int  This is the number of Generations that this process should repeat.
	 * @paran unsigned int  This is the number of epochs the network will be trained.
	 * @param double        This is the learning rate for every network (assumed to be 0.01).
	 */
	void reinforcementSimulate(unsigned int,unsigned int, double=0.01);

	
    //Evolution functions.
    
	/**
	 * This function deletes the worst half of the networks in the population and crosses the best performing
	 * networks to replace the deleted ones.
	 */
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
