#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include "Genome.h"
#include "Node.h"
#include <vector>
#include <thread>
#include <string>


class NeuralNetwork
{
public:
     NeuralNetwork();
     NeuralNetwork(std::string);
     ~NeuralNetwork();
     void updateStructure();
     void loadFromFile(std::string);
     void saveNetwork(std::string);

     void runForward();

     void batchGradientDescent(double);

private:

     Genome * instructions;

     std::vector<Node*> input;
     std::vector<std::vector<Node*>> hiddenLayer;
     std::vector<Node*> output;
};

#endif
