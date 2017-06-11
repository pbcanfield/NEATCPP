#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork()
{
    generation = 0;
    dna = new Genome();
}

NeuralNetwork::NeuralNetwork(std::string dir)
{
    dna = new Genome();
    generation = 0;

    dna -> loadFromFile(dir);
    updateStructure();

}

NeuralNetwork::NeuralNetwork(Genome code)
{
    dna = new Genome();
    generation = 0;
    dna -> copyIntoGenome(code);
    updateStructure();
}

NeuralNetwork::~NeuralNetwork()
{
    delete dna;

    for(auto & node : inputs)
        delete node;

    for(auto & vec : hiddenLayer)
        for(auto & node : vec)
            delete node;

    for(auto & node : outputs)
        delete node;
}

void NeuralNetwork::updateStructure()
{
    //Create the nodes in the network.
    //Create the input layer.
    for(unsigned int i = 0; i < dna -> getInput(); ++i)
        inputs.push_back(new Node());

    //Create the hidden layer.
    std::vector<unsigned int> topo = dna -> getHidden();
    unsigned int size = topo.size();

    hiddenLayer.resize(size);
    for(unsigned int i = 0; i < size; ++i)
        for(unsigned int j = 0; j < topo[i]; ++j)
            hiddenLayer[i].push_back(new Node());

    //Create the output layer.
    for(unsigned int i = 0; i < dna -> getOutput(); ++i)
        outputs.push_back(new Node());

    //Connect structure based on the genes in the genome and add the correct weights.
    unsigned int numConnections = dna -> getGenes().size();
    Gene currentGene;
    Node * first;
    Node * last;
    for(unsigned int i = 0; i < numConnections; ++i)
    {
        currentGene = dna -> getGene(i);
        first = findNodeWithID(currentGene.inID);
        last = findNodeWithID(currentGene.outID);
        first -> addForward(last,first);
        weight = first -> getLastForward();
        weight -> value() = currentGene.weight;
        last -> addBackwards(weight);
    }

}

void NeuralNetwork::mutate()
{

}

void NeuralNetwork::saveNetwork(std::string name)
{
    name += ".charzar"; //maybe dont do it this way
    dna -> saveGenome(name);
}

void NeuralNetwork::loadFromFile(std::string dir)
{
    dna -> loadFromFile(dir);
    updateStructure();
}


//There is a better way to write this code;
Node * NeuralNetwork::findNodeWithID(unsigned int ID)
{
    //calculates the size of the hiddenLayer.
    unsigned int hiddenLayerSize = 0;
    for(auto & vec : hiddenLayer)
        hiddenLayerSize += vec.size();

    if(ID < inputs.size())
    {
        return inputs[ID];
    }
    else if (ID < inputs.size() + hiddenLayerSize)
    {
        ID -= inputs.size();
        unsigned int count = 0;

        for(auto & vec : hiddenLayer)
        {
            for(auto & node : vec)
            {
                if(count == ID)
                    return node;
                ++count;
            }
        }
    }
    else
    {
        ID -= inputs.size() + hiddenLayerSize;
        return outputs[ID];
    }
    return NULL; //If this works this will never execute.
}
