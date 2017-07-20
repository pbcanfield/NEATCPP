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
     //the first int that is written is the size of the hidden layer
     //the second int that is writtien is the number of biases
     //then the input, the hiddenLayer, and the outputlayer is written

     unsigned int castingVar = hiddenLayer.size();
     charizard.write((char*)&castingVar,INT);
     castingVar = biasInfo.size();
     charizard.write((char*)&castingVar,INT);

     charizard.write((char*)&input,INT);
     for(auto & layer: hiddenLayer)
          charizard.write((char*)&layer,INT);
     charizard.write((char*)&output,INT);

     for(auto & bias : biasInfo)
     {
         charizard.write((char*)&bias.bias,DOUBLE);
         charizard.write((char*)&bias.node,INT);
     }

     for(auto & gene: geneticCode)
     {
          charizard.write((char*)&gene.inID,INT);
          charizard.write((char*)&gene.outID,INT);
          charizard.write((char*)&gene.weight,DOUBLE);
          charizard.write((char*)&gene.generation,INT);
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
          unsigned int totalSize,biasSize;
          cry.read((char*)&totalSize,INT);
          cry.read((char*)&biasSize,INT);
          cry.read((char*)&input,INT);
          networkSize += input;

          unsigned int layer;
          for(unsigned int i = 0; i < totalSize; ++i)
          {
               cry.read((char*)&layer,INT);
               hiddenLayer.push_back(layer);
               networkSize += layer;
          }

          cry.read((char*)&output,INT);
          networkSize += output;

          Bias bTemp;
          for(unsigned int i = 0 ; i < biasSize; ++i)
          {
              cry.read((char*)&bTemp.bias,DOUBLE);
              cry.read((char*)&bTemp.node,INT);
              biasInfo.push_back(bTemp);
          }
          Gene gTemp;
          while(!cry.eof())
          {
               cry.read((char*)&gTemp.inID,INT);
               cry.read((char*)&gTemp.outID,INT);
               cry.read((char*)&gTemp.weight,DOUBLE);
               cry.read((char*)&gTemp.generation,INT);
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

void Genome::addBias(Bias info)
{
    biasInfo.push_back(info);
}

Bias & Genome::getBias(unsigned int pos)
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

std::vector<Bias> Genome::getBiasVector()
{
    return biasInfo;
}

Gene & Genome::getGene(unsigned int pos)
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
