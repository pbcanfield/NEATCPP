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
    Genome startingNetwork(input,output);
    std::vector<NeuralNetwork*> nets;
    for (unsigned int i = 0; i < populationSize; ++i)
        nets.push_back(new NeuralNetwork(startingNetwork));

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


/**
 * This is the function that sets the number of training iterations
 * will be run on a specific network.
 * @param vector The input data to the networks
 * @param vector The ourput data in the network.
 * @param int The number of times the network will train over the whole dataset.
 */
void NetworkManager::trainNetworksOnline(std::vector<std::vector<double>> inputData,
                                std::vector<std::vector<double>> trainingData,
                                unsigned int epochs, double learningRate)
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

}

/**
 * Applies the quicksort algorithm to the LMS performance of the networks.
 * @param vec   The current vector of NeuralNetwork pointers.
 * @param left  The starting position of the partition.
 * @param right The ending position of the partition.
 */
void NetworkManager::quicksortVector(std::vector<NeuralNetwork*> & vec, int start, int end)
{
    if(start < end)
    {
        unsigned int pIndex = partition(vec, start, end );
        quicksortVector(vec,start,pIndex - 1 );
        quicksortVector(vec, pIndex + 1, end );
    }
}

unsigned int NetworkManager::partition(std::vector<NeuralNetwork*> & vec, int start, int end)
{
    unsigned int pIn = start;
    double pivot = vec[end] -> getLMSError();

    for(int i = 0; i < end - 1; ++i)
    {
        if(vec[i] -> getLMSError() <= pivot)
        {
            NeuralNetwork * temp = vec[i];
            vec[i] = vec[pIn];
            vec[pIn] = temp;
            ++pIn;
        }
    }

    return pIn;
}
