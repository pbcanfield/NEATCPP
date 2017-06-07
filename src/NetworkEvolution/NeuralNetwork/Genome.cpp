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

     if(file.is_open())
     {
          //writes the metadata to the file
          //the first int that is written is the size of the metadata
          //then the input, the hiddenLayer, and the outputlayer is written

          unsigned int layerSize = hiddenLayer.size() + 2;
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
               charizard.write((char*)&gene.generation,sizeof(int));     
          }

          /*
          file << "--------Metadata--------" << std::endl;
          file << input;
          for(auto & size : hiddenLayer)
               file << ',' << size;
          file << ',' << output << std::endl;
          file << "--------Metadata--------" << std::endl;

          for(auto & gene : geneticCode)
               file << gene.inID << ',' << gene.outID << ',' << gene.weight
                    << ',' << gene.enabled << ',' << gene.generation << std::endl;
          */
     }
     else
     {
          std::cout << "Could not open file " << dir << std::endl;
     }
     file.close();
}

void Genome::loadFromFile(std::string dir)
{
     std::ifstream file(dir);
     std::string line;


     gotoLine(file,2);
     file >> line;



     gotoLine(file,4);

     while(!file.eof())
     {
          file >> line;
     }
     file.close();
}

void Genome::gotoLine(std::ifstream & file, unsigned int lineNum)
{
     file.seekg(std::ios::beg);
     std::string line;
     for(unsigned int i = 0; i < lineNum - 1; ++i)
          file >> line;
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
