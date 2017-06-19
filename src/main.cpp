#include <iostream>
#include <vector>
#include "NetworkEvolution/NetworkManager.h"



int main( int argc, char * argv[])
{

    //This is a test genome that I have wrote to a file for testing.

    Gene gene;
    double b;
    Genome code;
    std::vector<unsigned int> topo {3,2};
    code.setInput(2);
    code.setHidden(topo);
    code.setOutput(2);

    b = 0.5;
    code.addBias(b);
    code.addBias(b);
    code.addBias(b);
    code.addBias(b);
    code.addBias(b);
    code.addBias(b);
    code.addBias(b);
    code.addBias(b);
    code.addBias(b);
    //-----1-----
    gene.inID = 0;
    gene.outID = 2;
    gene.weight = 0.5;
    gene.enabled = true;
    code.addGene(gene);
    //-----2-----
    gene.inID = 1;
    gene.outID = 3;
    gene.weight = 0.5;
    gene.enabled = true;
    code.addGene(gene);
    //-----3-----
    gene.inID = 1;
    gene.outID = 4;
    gene.weight = 0.5;
    gene.enabled = true;
    code.addGene(gene);
    //-----4-----
    gene.inID = 2;
    gene.outID = 5;
    gene.weight = 0.5;
    gene.enabled = true;
    code.addGene(gene);
    //-----5-----
    gene.inID = 3;
    gene.outID = 5;
    gene.weight = 0.5;
    gene.enabled = true;
    code.addGene(gene);
    //-----6-----
    gene.inID = 4;
    gene.outID = 6;
    gene.weight = 0.5;
    gene.enabled = true;
    code.addGene(gene);
    //-----7-----
    gene.inID = 5;
    gene.outID = 7;
    gene.weight = 0.5;
    gene.enabled = true;
    code.addGene(gene);
    //-----8-----
    gene.inID = 5;
    gene.outID = 8;
    gene.weight = 0.5;
    gene.enabled = true;
    code.addGene(gene);
    //-----9-----
    gene.inID = 6;
    gene.outID = 8;
    gene.weight = 0.5;
    gene.enabled = true;
    code.addGene(gene);

    code.saveGenome("../TestGenome.charzar");

    NeuralNetwork testNetwork;
    testNetwork.loadFromFile("../TestGenome.charzar");
    return 0;
}
