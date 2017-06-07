#include <iostream>
#include <vector>
#include "NetworkEvolution/NetworkManager.h"
#include "NetworkEvolution/NeuralNetwork/Genome.h"


int main( int argc, char * argv[])
{
     Gene cool;
     cool.inID = 0;
     cool.outID = 1;
     cool.weight = 0.6;
     cool.enabled = true;
     cool.generation = 0;

     std::vector<unsigned int> topo {3,3};

     Genome test;

     test.setInput(4);
     test.setHidden(topo);
     test.setOutput(2);
     test.addGene(cool);
     test.saveGenome("test.charzar");
     test.loadFromFile("test.charzar");


     std::cout << test.getGene(0).inID << ' ' << test.getGene(0).outID <<
          ' ' << test.getGene(0).weight << ' '  << test.getGene(0).enabled << ' '  <<
          test.getGene(0).generation << std::endl;
     return 0;
}
