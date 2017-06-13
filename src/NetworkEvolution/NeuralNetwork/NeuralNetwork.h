#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "Genome.h"
#include "Node.h"
#include <vector>
#include <string>

class NeuralNetwork
{
public:
    NeuralNetwork();
    NeuralNetwork(std::string);
    NeuralNetwork(Genome);
    ~NeuralNetwork();

    void updateStructure();
    void updateGenome();
    void mutate();

    void saveNetwork(std::string);
    void loadFromFile(std::string);

private:
    Node * findNodeWithID(unsigned int);

    std::vector<Node*> inputs;
    std::vector<std::vector<Node*>> hiddenLayer;
    std::vector<Node*> outputs;
    Genome * dna;

    unsigned int generation;
};

#endif
