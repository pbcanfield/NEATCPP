#include "Genome.h"
#include <fstream>
#include <iostream>

Genome::Genome()
{

}

void Genome::addGene(Gene gene)
{
     geneticCode.push_back(gene);
}

void Genome::setInput(unsigned int input)
{
     this -> input = input;
}

void Genome::setHidden(std::vector<unsigned int> hidden)
{
     hiddenLayer = hidden;
}

void Genome::addInput()
{
     ++input;
}

void Genome::addHidden(unsigned int pos)
{
     ++hiddenLayer[pos];
}

void Genome::addOutput()
{
     ++output;
}

void Genome::setOutput(unsigned int output)
{
     this -> output = output;
}

void Genome::saveGenome(std::string dir)
{
     std::ofstream file(dir);

     if(file.is_open())
     {
          file << "--------Metadata--------" << std::endl;
          file << input;
          for(auto & size : hiddenLayer)
               file << '\t' << size << '\t';
          file << output << std::endl;
          file << "--------Metadata--------" << std::endl;
          for(auto & gene : geneticCode)
               file << gene.inID << '\t' << gene.outID << '\t' << gene.weight
                    << '\t' << gene.enabled << '\t' << gene.generation << std::endl;
     }
     else
     {
          std::cout << "Could not open file " << dir << std::endl;
     }
     file.close();
}

void Genome::loadFromFile(std::string dir)
{

}

Gene Genome::getGene(unsigned int pos)
{
     return geneticCode[pos];
}

unsigned int Genome::getInput()
{
     return input;
}

std::vector<unsigned int> Genome::getHidden()
{
     return hiddenLayer;
}

unsigned int Genome::getOutput()
{
     return output;
}
