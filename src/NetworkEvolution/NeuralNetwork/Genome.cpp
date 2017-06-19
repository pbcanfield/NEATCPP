#include "Genome.h"
#include <iostream>
/**
 * Author: Pete Canfield & Daiwei Chen
 * Date: 2017-6-15
 */

Genome::Genome()
{

}

Genome::Genome(std::string dir)
{
     loadFromFile(dir);
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

     for(auto & bias : biasInfo)
         charizard.write((char*)&bias,sizeof(double));

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
     biasInfo.clear();
     std::ifstream cry(dir, std::ios::binary);

     if(cry.is_open())
     {
          unsigned int networkSize = 0;
          unsigned int totalSize;
          cry.read((char*)&totalSize,sizeof(int));
          cry.read((char*)&input,sizeof(int));
          networkSize += input;

          unsigned int layer;
          for(unsigned int i = 0; i < totalSize; ++i)
          {
               cry.read((char*)&layer,sizeof(int));
               hiddenLayer.push_back(layer);
               networkSize += layer;
          }

          cry.read((char*)&output,sizeof(int));
          networkSize += output;

          double bTemp;
          for(unsigned int i = 0 ; i < networkSize; ++i)
              cry.read((char*)&bTemp,sizeof(double));
              biasInfo.push_back(bTemp);

          Gene gTemp;
          while(!cry.eof())
          {
               cry.read((char*)&gTemp.inID,sizeof(int));
               cry.read((char*)&gTemp.outID,sizeof(int));
               cry.read((char*)&gTemp.weight,sizeof(double));
               cry.read((char*)&gTemp.enabled,sizeof(bool));
               cry.read((char*)&gTemp.generation,sizeof(int));
               geneticCode.push_back(gTemp);
          }
          geneticCode.pop_back(); // this is a work around and needs to be fixed
          cry.close();
     }
     else
     {
          std::cout << "Could not open file \"" << dir << '\"' << std::endl;
     }
}

void Genome::addBias(double info)
{
    biasInfo.push_back(info);
}

double Genome::getBias(unsigned int pos)
{
    return biasInfo[pos];
}

unsigned int Genome::getBiasSize()
{
    return biasInfo.size();
}

void Genome::copyIntoGenome(Genome & code)
{
    geneticCode = code.getGenes();
    biasInfo = code.getBiasVector();
    input = code.getInput();
    hiddenLayer = code.getHidden();
    output = code.getOutput();

}

std::vector<Gene> Genome::getGenes()
{
    return geneticCode;
}

std::vector<double> Genome::getBiasVector()
{
    return biasInfo;
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
