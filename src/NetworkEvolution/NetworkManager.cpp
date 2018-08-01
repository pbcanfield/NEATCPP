#include "NetworkManager.h"
#include <iostream>


/**
 * This is the default constructor which is resposible for creating an empty
 * NetworkManager.
 */
NetworkManager::NetworkManager()
{

}


/**
 * This is the constructor that generates a population of minimal structure
 * neural networks.
 * @param populationSize This is the amount of networks that will be in the
 *                       whole group of networks.
 * @param input          The number of input nodes for each network.
 * @param output         The number of output nodes for each network.
 */
NetworkManager::NetworkManager(unsigned int populationSize, unsigned int input,
                               unsigned int output)
{
    std::vector<NeuralNetwork*> nets;
    for (unsigned int i = 0; i < populationSize; ++i)
    {
        Genome startingNetwork(input,output);
        nets.push_back(new NeuralNetwork(startingNetwork,rand()));
    }
    networks.push_back(nets);
}


/**
 * This is the deconstructor that cleans up after the creation of each
 * NeuralNetwork.
 */
NetworkManager::~NetworkManager()
{
     for(auto & vec : networks)
          for(auto & net : vec)
               delete net;
}


void NetworkManager::setNetworkInputs(std::vector<std::vector<double>> data)
{
	inputData = data;
}


void NetworkManager::setNetworkTraining(std::vector<std::vector<double>> data)
{
	trainingData = data;
}

void NetworkManager::setMutationProbability(float overall, float node,
											float bias, float connection)
{
	mutationProb = overall;
	nodeAddProb = node;
	biasAddProb = bias;
	connectionProb = connection;
}

/**
 * This is the function that sets the number of training iterations
 * will be run on a specific network.
 * @param vector The input data to the networks
 * @param vector The ourput data in the network.
 * @param int The number of times the network will train over the whole dataset.
 */
void NetworkManager::trainNetworksOnline(unsigned int epochs, double learningRate)
{
	if(trainingData.size() > 0 && inputData.size() > 0)
    {
        unsigned int size = inputData.size();
        for(auto & vec : networks)
        {
            for(auto & neuralNetwork : vec)
            {
                for(unsigned int i = 0; i < epochs; ++i)
                {
                    for(unsigned int j = 0; j < size; ++j)
                    {
                        neuralNetwork -> setInputs(inputData[j]);
                        neuralNetwork -> setTraining(trainingData[j]);
                        neuralNetwork -> runForward();
                        neuralNetwork -> gradientDecent(learningRate);
                    }
                }
            }
        }
    }
    else
        std::cout << "There is no data in the input or training vector" << std::endl;
}


/**
 * This is the function that will be resposible for sorting the networks in
 * accordance to their least mean sqaured error from highest performance to
 * lowest performance.
 */
void NetworkManager::sortSupervisedNetworks()
{
    for(auto & vec : networks)
        quicksortVector(vec,0,vec.size() - 1);

}

void NetworkManager::crossTopHalf()
{
	unsigned int start,size;
	bool isOdd;
	for(auto & vec : networks)
	{
		size = vec.size();
		
		if(size % 2 == 0)
		{
			start = size/2;
			isOdd = false;
		}
		else
		{
			start = (size + 1)/2;
			isOdd = true;
		}

		for(unsigned int i = start; i < size; ++i)
			vec.pop_back();

		size = vec.size();
		
		if(isOdd)
			for(unsigned int i = 0; i < start - 1; ++i)
				vec.push_back(*vec[i] + vec[i + 1]);
		else
			for(unsigned int i = 0; i < start; ++i)
				vec.push_back(*vec[i] + vec[i + 1]);
	}
}


/**
 * Simulates the entire population of NeuralNetworks over a number of generations.
 * Based ona reinforcment learning process.
 * @param numGenerations The number of generations that should be simulated.
 */
void NetworkManager::reinforcementSimulate(unsigned int numGenerations,unsigned int epochs, double lr)
{
	for(unsigned int i = 0; i < numGenerations; ++i)
	{
		for(auto & vec : networks)
		{
			for(auto & network : vec)
			{
				network -> updateGeneration();
				if(isMutation())
						network -> randomMutation(nodeAddProb,biasAddProb,connectionProb);
			}
		}
		trainNetworksOnline(epochs,lr);
		sortSupervisedNetworks();
		crossTopHalf();
		std::cout << "Done simulating: " << i + 1 << " out of " << numGenerations << " generations [";
		std::cout << i * 100. /numGenerations << "% Done]" << std::endl;
	}
	std::cout << "Done Simulating" << std::endl;
}

/**
 * Applies the quicksort algorithm to the LMS performance of the networks.
 * @param vec   The current vector of NeuralNetwork pointers.
 * @param left  The starting position of the partition.
 * @param right The ending position of the partition.
 */
void NetworkManager::quicksortVector(std::vector<NeuralNetwork*> & arr, int left, int right)
{
	int min = left + (right - left) / 2;

    int i = left;
    int j = right;
    double pivot = arr[min] -> getLMSError();

    while(left<j || i<right)
    {
        while(arr[i] -> getLMSError() <pivot)
        i++;
        while(arr[j] -> getLMSError() >pivot)
        j--;

        if(i<=j){
            NeuralNetwork * temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
        else{
            if(left<j)
                quicksortVector(arr, left, j);
            if(i<right)
                quicksortVector(arr,i,right);
            return;
        }
    }
}

/**
 * returns true or false in accordance to the Mutation probabilty that is
 * used to determine the frequency of random mutations.
 */
bool NetworkManager::isMutation()
{
	return mutationProb <= rand()/(double)RAND_MAX;
}
