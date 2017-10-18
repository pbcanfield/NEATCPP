/**
 * Author: Pete Canfield & Daiwei Chen
 * Date: 2017-6-15
 */
#include "Genome.h"
#include <iostream>
#include <cstddef>

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
    for(unsigned int i = 0; i < nInput; ++i)
        input.push_back(i);

    for(unsigned int i = nInput; i < nOutput + nInput; ++i)
        output.push_back(i);

    Gene _gene;
    _gene.generation = 0;
    _gene.enabled = true;


    for(unsigned int i = 0; i < nInput; ++i)
    {
        for(unsigned int j = nInput; j < nOutput + nInput; ++j)
        {
            _gene.weight = randomNumber();
            _gene.inID = i;
            _gene.outID = j;
            geneticCode.push_back(_gene);
        }
    }
    ln = nOutput + nInput - 1;
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
 * This is resposible for crossing the genetic code of Nerural networks so
 * high perfoming NeuralNetworks can be bread.
 * @param code This is the code of the new network.
 */
void Genome::cross(Genome & code)
{

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
void Genome::setInput(std::vector<unsigned int> in)
{
     input = in;
}


/**
 * This is the function that sets the number of hidden nodes in the Genome.
 * It takes an std::vector of unsigned integers that are the number of nodes
 * in each layer of the hidden layer.
 */
void Genome::setHidden(std::vector<std::vector<unsigned int>> hidden)
{
     hiddenLayer = hidden;
}


/**
 * This is the function that sets the number of output nodes in the Genome.
 * It takes an unsigned integer that is the number of output nodes.
 */
void Genome::setOutput(std::vector<unsigned int> out)
{
     output = out;
}


//This is function that incraments the neumber of input nodes in the genome.
void Genome::addInput(unsigned int ID)
{
    input.push_back(ID);
}


//This is a function that incraments a specific layer of the hiddenLayer.
void Genome::addHidden(unsigned int pos,unsigned int ID)
{
    hiddenLayer[pos].push_back(ID);
}

/**
 * This is a function that is used to insert a node into a new layer. It takes
 * an unsigned int that is the layer that the new layer is to be insterted at
 * and adds one node to that location in a new layer.
 */
void Genome::insertHidden(unsigned int layer,unsigned int ID)
{
    std::vector<unsigned int> _vec {ID};
    hiddenLayer.insert(hiddenLayer.begin() + layer, _vec);
}


//This is function that incraments the neumber of output nodes in the genome.
void Genome::addOutput(unsigned int ID)
{
    output.push_back(ID);
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

    /**
     * writes the metadata to the file
     * The first int that is written is the size of the input layer.
     * The second int that is the number of layers in the hidden layer.
     * The next int is the number of nodes in the corrisponding layer.
     * The same pattern is followed for each layer.
     * The last int is the size of the output layer and each proceeding
     * int is the ID of each node.
     * The Next thing (INT) that is written is the number of bias nodes.
     * Then all the Biases are written to the file (BIAS).
     * The last node is then wrttien to the file (INT).
     * Then everything left in the file is the Gene connections between nodes.
     */

    unsigned int castingVar = input.size();
    charizard.write((char*)&castingVar,INT);
    for(auto & ID : input)
        charizard.write((char*)&ID,INT);

    castingVar = hiddenLayer.size();
    charizard.write((char*)&castingVar,INT);

    for(auto & vec : hiddenLayer)
    {
        castingVar = vec.size();
        charizard.write((char*)&castingVar,INT);

        for(auto & ID : vec)
            charizard.write((char*)&ID,INT);
    }

    castingVar = output.size();
    charizard.write((char*)&castingVar,INT);
    for(auto & ID : output)
        charizard.write((char*)&ID,INT);

    castingVar = biasInfo.size();
    charizard.write((char*)&castingVar,INT);

    //Write ln.
    charizard.write((char*)&ln,INT);

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
        charizard.write((char*)&gene.enabled,BOOL);
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
    input.clear();
    for(auto & vec : hiddenLayer)
        vec.clear();
    output.clear();
    hiddenLayer.clear();

    geneticCode.clear();
    biasInfo.clear();

    std::ifstream cry(dir, std::ios::binary);

    if(cry.is_open())
    {
        unsigned int hSize,size;
        cry.read((char*)&size,INT);

        unsigned int _ID;
        for(unsigned int i = 0; i < size; ++i)
        {
            cry.read((char*)&_ID,INT);
            input.push_back(_ID);
        }

        cry.read((char*)&size,INT);
        std::vector<unsigned int> _vec;
        for(unsigned int i = 0; i < size; ++i)
        {
            cry.read((char*)&hSize,INT);
            hiddenLayer.push_back(_vec);
            for(unsigned int j = 0; j < hSize; ++j)
            {
                cry.read((char*)&_ID,INT);
                hiddenLayer[i].push_back(_ID);

            }
        }

        cry.read((char*)&size,INT);
        for(unsigned int i = 0; i < size; ++i)
        {
            cry.read((char*)&_ID,INT);
            output.push_back(_ID);
        }

        cry.read((char*)&ln,INT);
        cry.read((char*)&size,INT);

        Bias bTemp;
        for(unsigned int i = 0 ; i < size; ++i)
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
            cry.read((char*)&gTemp.enabled,BOOL);
            cry.read((char*)&gTemp.generation,INT);
            geneticCode.push_back(gTemp);

            if(cry.eof())
                break;

        }
        cry.close();
    }
    else
        std::cout << "Could not open file \"" << dir << '\"' << std::endl;
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
	ln = code.lastNode();
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


/**
 * Gets a gene between two node ID's
 * @param  in  [description]
 * @param  out [description]
 * @return     [description]
 */
Gene & Genome::getGene(unsigned int in, unsigned int out)
{
    unsigned int pos = 0;
    unsigned int size = geneticCode.size();
    bool found = false;
    for(unsigned int i = 0; i < size && !found; ++i)
    {
        if(geneticCode[i].inID == in && geneticCode[i].outID == out)
        {
            pos = i;
            found = true;
        }
    }
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


//Returns the last node number that was created.
unsigned int & Genome::lastNode()
{
    return ln;
}

//This returns the number of input nodes.
std::vector<unsigned int> Genome::getInput()
{
     return input;
}


//This returns the vector that stores the hiddenLayer topology.
std::vector<std::vector<unsigned int>> Genome::getHidden()
{
     return hiddenLayer;
}


//This returns the number of output nodes.
std::vector<unsigned int> Genome::getOutput()
{
     return output;
}


bool Genome::geneExist(unsigned int start, unsigned int end)
{	
	for(auto & gene : geneticCode)
		if(gene.inID == start && gene.outID == end)
			return true;
	
	return false;
}

double Genome::randomNumber()
{
    double num = (double)(rand() % 10);
    num += rand() / (double)RAND_MAX;
    if(rand() % 2 == 0)
        num *= -1.0;
    return num;
}

/**
 * Checks if a Gene is the same as as Gene at a position in the
 * Gene vector.
 * @param  Gene This is the Gene being compared.
 * @param  int  This is the position that the Gene is at in the
 *              Gene vector.
 * @return      Returns true if the Gene is the same and false if
 *              it isnt.
 */
bool Genome::isSimilarGene(Gene connect,unsigned int pos)
{
    return connect.inID == geneticCode[pos].inID &&
           connect.outID == geneticCode[pos].outID;
}


/**
 * Checks if a Bias is the same as a Bias in the Bias
 * vector.
 * @param  bias This is the Bias being compared.
 * @param  pos  This is the position that the Bias is at in the
 *              Bias vector.
 * @return      Returns true if the Bias is the same and false if
 *              it isnt.
 */
bool Genome::isSimilarBias(Bias bias,unsigned int pos)
{
    return bias.node == biasInfo[pos].node;
}

