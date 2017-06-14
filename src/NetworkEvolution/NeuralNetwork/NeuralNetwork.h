#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "Genome.h"
#include "Node.h"
#include <vector>
#include <string>
#include <thread>

class NeuralNetwork
{
public:
    NeuralNetwork();
    NeuralNetwork(std::string);
    NeuralNetwork(Genome);
    ~NeuralNetwork();

    void updateStructure();
    void mutate();

    void runForward(unsigned int=0);

    void saveNetwork(std::string);
    void loadFromFile(std::string);

private:
    Node * findNodeWithID(unsigned int);
    static void processForward(unsigned int);
    unsigned int findNumInLayer(unsigned int);
    std::vector<Node*> & getLayer(unsigned int);

    std::vector<Node*> inputs;
    std::vector<std::vector<Node*>> hiddenLayer;
    std::vector<Node*> outputs;
    Genome * dna;

    unsigned int generation;

    //multithreading
    std::vector<std::thread*> threads;
    std::vector<bool> completed;
    unsigned int layerProcessed;
};

#endif
