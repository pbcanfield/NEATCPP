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

     cool.inID = 53;
     cool.outID = 10;
     cool.weight = 0.9;
     cool.enabled = false;
     cool.generation = 100;

     for(unsigned int i = 0; i < 100; ++i)
          test.addGene(cool);

     test.saveGenome("test.charzar");
     test.loadFromFile("test.charzar");

     for(unsigned int i = 0; i < test.getGenomeSize(); ++i)
          std::cout << test.getGene(i).inID << ' ' << test.getGene(i).outID <<
               ' ' << test.getGene(i).weight << ' '  << test.getGene(i).enabled << ' '  <<
               test.getGene(i).generation << std::endl;
     return 0;
}
