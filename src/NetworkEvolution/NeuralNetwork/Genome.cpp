#include "Genome.h"
#include <iostream>
#include <string>

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
     std::ofstream charizard(dir, std::ios::binary);

     //writes the metadata to the file
     //the first int that is written is the size of the metadata
     //then the input, the hiddenLayer, and the outputlayer is written

     unsigned int layerSize = hiddenLayer.size();
     charizard.write((char*)&layerSize, sizeof(int));
     charizard.write((char*)&input,sizeof(int));
     for(auto & layer: hiddenLayer)
          charizard.write((char*)&layer,sizeof(int));
     charizard.write((char*)&output,sizeof(int));

     for(auto & gene: geneticCode)

     {
          charizard.write((char*)&gene.inID,sizeof(int));
          charizard.write((char*)&gene.outID,sizeof(int));
          charizard.write((char*)&gene.weight,sizeof(double));
          charizard.write((char*)&gene.enabled,sizeof(bool));
          charizard.write((char*)&gene.generation,sizeof(int));
     }

     charizard.close();
}

void Genome::loadFromFile(std::string dir)
{
     hiddenLayer.clear();
     geneticCode.clear();
     std::ifstream cry(dir, std::ios::binary);

     if(cry.is_open())
     {
          unsigned int totalSize;
          cry.read((char*)&totalSize,sizeof(int));
          cry.read((char*)&input,sizeof(int));

          unsigned int layer;
          for(unsigned int i = 0; i < totalSize; ++i)
          {
               cry.read((char*)&layer,sizeof(int));
               hiddenLayer.push_back(layer);
          }

          cry.read((char*)&output,sizeof(int));

          Gene temp;
          while(!cry.eof())
          {
               cry.read((char*)&temp.inID,sizeof(int));
               cry.read((char*)&temp.outID,sizeof(int));
               cry.read((char*)&temp.weight,sizeof(double));
               cry.read((char*)&temp.enabled,sizeof(bool));
               cry.read((char*)&temp.generation,sizeof(int));
               geneticCode.push_back(temp);
          }
          geneticCode.pop_back(); // this is a work around and needs to be fixed
     }
     else
     {
          std::cout << "Could not open file " << dir << std::endl;
     }
}


void Genome::copyIntoGenome(Genome & code)
{
    geneticCode = code.getGenes();
    input = code.getInput();
    hiddenLayer = code.getHidden();
    output = code.getOutput();
}

std::vector<Gene> Genome::getGenes()
{
    return geneticCode;
}

Gene Genome::getGene(unsigned int pos)
{
     return geneticCode[pos];
}

unsigned int Genome::getGenomeSize()
{
     return geneticCode.size();
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
