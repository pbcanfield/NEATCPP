/**
 * Author: Pete Canfield & Daiwei Chen
 * Date: 2017-6-15
 */
#include "Genome.h"
#include <iostream>

/**
 * This is the defualt constructor that doesnt do anything really but it is
 * important becuase it allows the user to create an empty genome.
 */
Genome::Genome()
{

}


/**
 * This constructor creates a specfic type of genome that is usefull for the
 * NEAT system. It generates a genome that specifies a fully connected network
 * between input and output nodes without and bias nodes.
 */
Genome::Genome(unsigned int nInput, unsigned int nOutput)
{
    input = nInput;
    output = nOutput;
    Gene _gene;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-10.0,10.0);

    for(unsigned int i = 0; i < nInput; ++i)
    {
        for(unsigned int j = nInput; j < nOutput + nInput; ++j)
        {
            //Generate a random weight here, needs to be implamented in the future.
            _gene.weight = dis(gen);
            _gene.inID = i;
            _gene.outID = j;
            _gene.generation = 0;
            geneticCode.push_back(_gene);
        }
    }
}


/**
 * This constructor takes a directory and loads a genome into memory from a
 * charzar.
 */
Genome::Genome(std::string dir)
{
     loadFromFile(dir);
}


/**
 * This function pushes back a gene into the genome gene vector. The function
 * takes a Gene which serves as the gene that is being pushed back into the
 * vector.
 */
void Genome::addGene(Gene gene)
{
     geneticCode.push_back(gene);
}


/**
 * This function pushes back a bias into the genome bais vector. The function
 * takes a Bias which serves as the bais that is being pushed back into the
 * vector.
 */
void Genome::addBias(Bias info)
{
    biasInfo.push_back(info);
}


/**
 * This is the function that sets the number of input nodes in the Genome.
 * It takes an unsigned integer that is the number of input nodes.
 */
void Genome::setInput(unsigned int in)
{
     input = in;
}


/**
 * This is the function that sets the number of hidden nodes in the Genome.
 * It takes an std::vector of unsigned integers that are the number of nodes
 * in each layer of the hidden layer.
 */
void Genome::setHidden(std::vector<unsigned int> hidden)
{
     hiddenLayer = hidden;
}


/**
 * This is the function that sets the number of output nodes in the Genome.
 * It takes an unsigned integer that is the number of output nodes.
 */
void Genome::setOutput(unsigned int out)
{
     output = out;
}

/**
 * This function updates the values of the IDs for all genes and biases after
 * there is an insertion of a node by the mutateAddNode function in the
 * NeuralNetwork.
 */
void Genome::updateConnectionStructure(unsigned int newNodeID)
{
    //This can probably be made way more efficient.
    for(auto & gene : geneticCode)
    {
        if(gene.inID >= newNodeID)
            ++gene.inID;
        if(gene.outID >= newNodeID)
            ++gene.outID;
    }

    for(auto & bias : biasInfo)
    {
        if (bias.node >= newNodeID)
            ++bias.node;
    }
}


/**
 * This is a function that removes a Gene from the gene vector in the Genome
 * based on the two node ID's it is connected to.
 */
void Genome::removeConnection(unsigned int in ,unsigned int out)
{
    unsigned int size = geneticCode.size();
    bool found = false;
    for(unsigned int i = 0; i < size && !found; ++i)
    {
        if(geneticCode[i].inID == in && geneticCode[i].outID == out)
        {
            found = true;
            geneticCode.erase(geneticCode.begin() + i);
        }
    }
}


//This is function that incraments the neumber of input nodes in the genome.
void Genome::addInput()
{
     ++input;
}


//This is a function that incraments a specific layer of the hiddenLayer.
void Genome::addHidden(unsigned int pos)
{
     ++hiddenLayer[pos];


}

/**
 * This is a function that is used to insert a node into a new layer. It takes
 * an unsigned int that is the layer that the new layer is to be insterted at
 * and adds one node to that location in a new layer.
 */
void Genome::insertHidden(unsigned int layer)
{
    hiddenLayer.insert(hiddenLayer.begin() + layer,1);
}


//This is function that incraments the neumber of output nodes in the genome.
void Genome::addOutput()
{
     ++output;
}

/**
 * The saveGenome function takes a directory to a file as a string and saves it
 * to that directory. The standard file extension is .charzar for this
 * implemention and the NeuralNetwork will force the file to have a .charzar
 * extension if it is not specified. The function uses binary reads and writes
 * and writes binary data pertaining to all data in the Genome directly to
 * blocks of space by the .write member function of std::ofstream.
 */
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
         charizard.write((char*)&bias.generation,INT);
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


/**
 * The loadFromFile function takes in the directory to a charzar file and reads
 * in the data from that file. The file must be from a machine with the same
 * architechture as differnces in variable sizes will lead to corrupt data.
 */
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
              cry.read((char*)&bTemp.generation,INT);
              biasInfo.push_back(bTemp);
          }
          Gene gTemp;
          while(true)
          {
               cry.read((char*)&gTemp.inID,INT);
               cry.read((char*)&gTemp.outID,INT);
               cry.read((char*)&gTemp.weight,DOUBLE);
               cry.read((char*)&gTemp.generation,INT);
               geneticCode.push_back(gTemp);

               if(cry.eof())
                    break;

          }
          cry.close();
     }
     else
     {
          std::cout << "Could not open file \"" << dir << '\"' << std::endl;
     }
}


/**
 * This function copies one genome into itself. It takes a reference to another
 * genome and copies all data into this genome.
 */
void Genome::copyIntoGenome(Genome & code)
{
    geneticCode = code.getGeneVector();
    biasInfo = code.getBiasVector();
    input = code.getInput();
    hiddenLayer = code.getHidden();
    output = code.getOutput();
}


//This function returns the Gene vector.
std::vector<Gene> Genome::getGeneVector()
{
    return geneticCode;
}


//This function returns the Bias vector.
std::vector<Bias> Genome::getBiasVector()
{
    return biasInfo;
}


//This returns a reference to a Gene at a position.
Gene & Genome::getGene(unsigned int pos)
{
     return geneticCode[pos];
}


//This returns a reference to a Bias at a position.
Bias & Genome::getBias(unsigned int pos)
{
    return biasInfo[pos];
}


//This returns the size of the Gene vector.
unsigned int Genome::getGenomeSize()
{
     return geneticCode.size();
}


//This returns the size of the Bias vector.
unsigned int Genome::getBiasSize()
{
    return biasInfo.size();
}


//This returns the number of input nodes.
unsigned int Genome::getInput()
{
     return input;
}


//This returns the vector that stores the hiddenLayer topology.
std::vector<unsigned int> Genome::getHidden()
{
     return hiddenLayer;
}


//This returns the number of output nodes.
unsigned int Genome::getOutput()
{
     return output;
}
