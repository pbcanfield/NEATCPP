#include <iostream>
#include <vector>
#include "NetworkEvolution/NetworkManager.h"



int main( int argc, char * argv[])
{

    //This is a test genome that I have wrote to a file for testing.
    //This is a test genome that I have wrote to a file for testing.
    Gene gene;


    Genome code;
    std::vector<unsigned int> topo {3,2};
    code.setInput(2);
    code.setHidden(topo);
    code.setOutput(2);

    //-----1-----
    gene.inID = 0;
    gene.outID = 2;
    gene.weight = 1;
    code.addGene(gene);
    //-----2-----
    gene.inID = 1;
    gene.outID = 3;
    gene.weight = 1;
    code.addGene(gene);
    //-----3-----
    gene.inID = 1;
    gene.outID = 4;
    gene.weight = 1;
    code.addGene(gene);
    //-----4-----
    gene.inID = 2;
    gene.outID = 5;
    gene.weight = 1;
    code.addGene(gene);
    //-----5-----
    gene.inID = 3;
    gene.outID = 5;
    gene.weight = 1;
    code.addGene(gene);
    //-----6-----
    gene.inID = 4;
    gene.outID = 6;
    gene.weight = 1;
    code.addGene(gene);
    //-----7-----
    gene.inID = 5;
    gene.outID = 7;
    gene.weight = 1;
    code.addGene(gene);
    //-----8-----
    gene.inID = 5;
    gene.outID = 8;
    gene.weight = 1;
    code.addGene(gene);
    //-----9-----
    gene.inID = 6;
    gene.outID = 8;
    gene.weight = 1;
    code.addGene(gene);


    code.saveGenome("../blueprints/TestGenome.charzar");


    NeuralNetwork testNetwork;
    testNetwork.loadFromFile("../blueprints/TestGenome.charzar");

    std::vector<double> val {0.3,0.7};
    std::vector<double> out;
    testNetwork.setInputs(val);
    val[0] = 0.99;
    val[1] = 1;
    testNetwork.visualize(900,600,1);
    testNetwork.setTraining(val);
    testNetwork.runForward();


    std::cout << "\nBefore optimization\n" << std::endl;
    std::cout << "Total Error: " << testNetwork.getLMSError() << std::endl;
    out = testNetwork.getNetworkOutput();
    for(unsigned int i = 0; i < out.size(); ++i)
        std::cout << "out: " << out[i] << " target " << val[i] << std::endl;


    for(unsigned int i = 0; i < 10000000; ++i)
    {
        testNetwork.gradientDecent(0.01);
        testNetwork.runForward();
    }


    testNetwork.mutateAddBias(2);
    testNetwork.runForward();


    out = testNetwork.getNetworkOutput();
    std::cout << "\nAfter optimization\n" <<std::endl;
    for(unsigned int i = 0; i < out.size(); ++i)
        std::cout << "out: " << out[i] << " target " << val[i] << std::endl;
    std::cout << "Total Error: " << testNetwork.getLMSError() << std::endl;


    testNetwork.saveNetwork("../blueprints/testNetwork");

    testNetwork.loadFromFile("../blueprints/testNetwork.charzar");
    testNetwork.runForward();

    out = testNetwork.getNetworkOutput();
    std::cout << "\nAfter optimization\n" <<std::endl;
    for(unsigned int i = 0; i < out.size(); ++i)
        std::cout << "out: " << out[i] << " target " << val[i] << std::endl;
    std::cout << "Total Error: " << testNetwork.getLMSError() << std::endl;


    return 0;
}
