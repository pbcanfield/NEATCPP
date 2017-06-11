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

    //This could potentially be very inefficient.
    inputs.clear();

    for(auto & vec : hiddenLayer)
        vec.clear();
    hiddenLayer.clear();

    outputs.clear();
    //Create the nodes in the network.
    //Create the input layer.
    NodeInfo currentNode;
    unsigned int alreadyProcessed = dna -> getInput();
    for(unsigned int i = 0; i < alreadyProcessed; ++i)
    {
        currentNode = dna -> getNode(i);
        inputs.push_back(new Node(currentNode.value,currentNode.bias));
    }
    //Create the hidden layer.
    std::vector<unsigned int> topo = dna -> getHidden();
    unsigned int size = topo.size();

    hiddenLayer.resize(size);

    for(unsigned int i = 0; i < size; ++i)
    {
        for(unsigned int j = 0; j < topo[i]; ++j)
        {
            currentNode = dna -> getNode(alreadyProcessed);
            hiddenLayer[i].push_back(new Node(currentNode.value,currentNode.bias));
            ++alreadyProcessed;
        }
    }
    //Create the output layer.
    for(unsigned int i = 0; i < dna -> getOutput(); ++i)
    {
        currentNode = dna -> getNode(alreadyProcessed);
        outputs.push_back(new Node(currentNode.value,currentNode.bias));
        ++alreadyProcessed;
    }
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
        first -> addForward(last,first,currentGene.weight);
        last -> addBackwards(first -> getLastForward());
    }
}

void NeuralNetwork::updateGenome()
{
    dna -> setInput(inputs.size());
    std::vector<unsigned int> topology(hiddenLayer.size());
    for(auto & vec : hiddenLayer)
        topology.push_back(vec.size());
    dna -> setHidden(topology);
    dna -> setOutput(outputs.size());
}

void NeuralNetwork::mutate()
{

}

void NeuralNetwork::saveNetwork(std::string name)
{
    name += ".charzar"; //maybe dont do it this way
    updateGenome();
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
