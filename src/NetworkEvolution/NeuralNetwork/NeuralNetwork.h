#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "Genome.h"
#include "Node.h"
#include <vector>
#include <math.h>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>


class NeuralNetwork
{
public:
    NeuralNetwork();
    NeuralNetwork(std::string);
    NeuralNetwork(Genome);
    ~NeuralNetwork();

    void updateStructure();
    void mutate();

    void setInputs(std::vector<double>);
    void setTraining(std::vector<double>);
    void runForward();
    void runForward(unsigned int);
    void gradientDecent(double);


    double getLMSError();

    void saveNetwork(std::string);
    void loadFromFile(std::string);
    std::vector<double> getNetworkOutput();

private:
    Node * findNodeWithID(unsigned int);
    void processForward(unsigned int,unsigned int);
    unsigned int findNumInLayer(unsigned int);
    std::vector<Node*> & getLayer(unsigned int);
    void lockFunc(std::atomic<unsigned int> &, unsigned int);

    void updateGene(Node *, unsigned int &);
    void updateBias(Node *, unsigned int &);

    std::vector<Node*> inputs;
    std::vector<std::vector<Node*>> hiddenLayer;
    std::vector<Node*> outputs;
    std::vector<double> training;

    Genome * dna;

    unsigned int generation;

    //multithreading
    std::vector<std::thread*> threads;
    std::mutex mLock;
    std::atomic<unsigned int> completed;
};

#endif
