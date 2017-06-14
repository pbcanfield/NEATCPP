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


void NeuralNetwork::mutate()
{

}

void NeuralNetwork::runForward(unsigned int numThreads)
{
    layerProcessed = 1; //start at the first hidden layer.

    //This is where the multithreading starts.
    for(unsigned int i = 0; i < numThreads; ++i)
    {
        threads.push_back(new std::thread(processForward,i));
        completed.push_back(false);
    }

    // It should be +2 but im saving a computation because
    // 1 will be subtracted later.
    unsigned int totalLayers = hiddenLayer.size() + 1;
    unsigned int checkNum = 0;
    bool mainComplete = false;

    unsigned int rem;
    while(layerProcessed != totalLayers)
    {
        if(!mainComplete)
        {
            unsigned int nLayer = findNumInLayer(layerProcessed);
            rem = nLayer & numThreads;
            if(rem == 0)
                mainComplete = true;
            else
            {
                std::vector<Node*> & currentLayer = getLayer(layerProcessed);
                for(unsigned int i = nLayer - rem; i < nLayer; ++i)
                    currentLayer[i] -> calculate();

            }
        }
        //check if every thread has completed;
        for(auto status : completed)
            if(status)
                ++checkNum;

        if(checkNum == numThreads)
        {
            checkNum = 0;
            for(unsigned int i = 0; i < completed.size(); ++i)
                completed[i] = false;
            mainComplete = false;
            ++layerProcessed;
        }


    }


    for(auto & thread : threads)
    {
        thread -> join();
        delete thread;
    }

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

    ID -= inputs.size() + hiddenLayerSize;
    return outputs[ID];


}

void NeuralNetwork::processForward(unsigned int ID)
{

}

unsigned int NeuralNetwork::findNumInLayer(unsigned int layer)
{
    unsigned int nLayer = outputs.size();
    if (layer == 0)
        nLayer = inputs.size();
    else if(layer <= hiddenLayer.size())
        nLayer = hiddenLayer[layer - 1].size();

    return nLayer;

}

std::vector<Node*> & NeuralNetwork::getLayer(unsigned int pos)
{
    if (pos == 0)
        return inputs;
    else if (pos <= hiddenLayer.size())
        return hiddenLayer[pos - 1];

    return outputs;
}
