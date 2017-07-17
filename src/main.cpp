#include <iostream>
#include <vector>
#include "NetworkEvolution/NetworkManager.h"



int main( int argc, char * argv[])
{

    //This is a test genome that I have wrote to a file for testing.
    /*
    Gene gene;
    Bias bias;
    Genome code;
    std::vector<unsigned int> topo {3,2};
    code.setInput(2);
    code.setHidden(topo);
    code.setOutput(2);

    bias.b = 0.5;
    bias.layer = 1;
    code.addBias(bias);
    bias.layer = 3;
    code.addBias(bias);
    //-----1-----
    gene.inID = 0;
    gene.outID = 2;
    gene.weight = 1;
    gene.enabled = true;
    code.addGene(gene);
    //-----2-----
    gene.inID = 1;
    gene.outID = 3;
    gene.weight = 1;
    gene.enabled = true;
    code.addGene(gene);
    //-----3-----
    gene.inID = 1;
    gene.outID = 4;
    gene.weight = 1;
    gene.enabled = true;
    code.addGene(gene);
    //-----4-----
    gene.inID = 2;
    gene.outID = 5;
    gene.weight = 1;
    gene.enabled = true;
    code.addGene(gene);
    //-----5-----
    gene.inID = 3;
    gene.outID = 5;
    gene.weight = 1;
    gene.enabled = true;
    code.addGene(gene);
    //-----6-----
    gene.inID = 4;
    gene.outID = 6;
    gene.weight = 1;
    gene.enabled = true;
    code.addGene(gene);
    //-----7-----
    gene.inID = 5;
    gene.outID = 7;
    gene.weight = 1;
    gene.enabled = true;
    code.addGene(gene);
    //-----8-----
    gene.inID = 5;
    gene.outID = 8;
    gene.weight = 1;
    gene.enabled = true;
    code.addGene(gene);
    //-----9-----
    gene.inID = 6;
    gene.outID = 8;
    gene.weight = 1;
    gene.enabled = true;
    code.addGene(gene);

    code.saveGenome("../TestGenome.charzar");
    */

    NeuralNetwork testNetwork;
    testNetwork.loadFromFile("../TestGenome.charzar");

    std::vector<double> val {0.5,0.25};
    testNetwork.setInputs(val);
    val[0] = 0.33;
    val[1] = 0.42;

    testNetwork.setTraining(val);
    testNetwork.runForward();

    std::cout << "Total Error: " << testNetwork.getLMSError() << std::endl;
    for(unsigned int i = 0; i < 1000; ++i)
    {
        testNetwork.gradientDecent(0.5);
        testNetwork.runForward();
    }
    std::cout << "Total Error: " << testNetwork.getLMSError() << std::endl;

    return 0;
}
