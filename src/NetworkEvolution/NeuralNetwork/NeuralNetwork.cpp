#include "NeuralNetwork.h"

/**
 * This is the defualt constructor that sets thegeneration to zero
 * and allocates the memory for the genome of this Neural Network.
 */
NeuralNetwork::NeuralNetwork()
{
    generation = 0;
    dna = new Genome();
}

/**
 * This is a constructor that takes a string that will automatically
 * load the genome from a specified charzar file and updates structure
 * accordingly.
 */
NeuralNetwork::NeuralNetwork(std::string dir)
{
    dna = new Genome();
    generation = 0;

    dna -> loadFromFile(dir);
    updateStructure();

}


/**
 * This is a constructor that takes a genome and copies that genome
 * into this networks genome and updates the structure accordingly.
 */
NeuralNetwork::NeuralNetwork(Genome code)
{
    dna = new Genome();
    generation = 0;
    dna -> copyIntoGenome(code);
    updateStructure();
}

/**
 * This destructor deallocates all the input, hidden and output
 * memory that was allocated for the network.
 */
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

/**
 * This is a member function that is responisble for analyzing
 * the structure of the genome and for emmulating that structure
 * in the neural network.
 */

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

/**
 * This is the function that is responisble for handling
 * the multithreading that runs the forward propgation
 * for the neural network.
 */
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

/**
 * This function saves the current genome to a charzar file.
 * It takes a string that is the directory of the file and the
 * charzar extension is automatically attatched to the file.
 */
void NeuralNetwork::saveNetwork(std::string name)
{
    name += ".charzar"; //maybe dont do it this way
    dna -> saveGenome(name);
}


/**
 * This is a function that loads a neural network from a file into memory.
 * It takes a string to the directory where the genome is saved and then
 * runs the updateStructure function which updates the struncture of the
 * Neural Network.
 */
void NeuralNetwork::loadFromFile(std::string dir)
{
    dna -> loadFromFile(dir);
    updateStructure();
}


/**
 * This is a function that takes a linear identification number and searches
 * the non-linear network structure for the corrisponding node.
 * It takes an unsigned in wich is an ID and returns a node pointer
 * which is the pointer to that specific node in the network structure.
 */
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


/**
 * This is a function that takes a thread Id and then calculates and runs the specific
 * forward propogation calculations that are neccissary for that thread to complete for
 * the specific layer. The layer that is currently being processed is managed by the
 * main thread in the runForward function.
 * It takes an unsinged int as an input which represents which thread is running this function.
 */
void NeuralNetwork::processForward(unsigned int ID)
{

}


/**
 * This is a fiunction that gets the total number of nodes in a specific layer of the
 * Neural Network.
 * It takes in a unsigned int that represents the layer number and returns
 * an unsigned int which is the total number of nodes in that layer.
 */
unsigned int NeuralNetwork::findNumInLayer(unsigned int layer)
{
    return getLayer(layer).size();
}

/**
 * This is a function that returns a layer at a specific position in the
 * Neural Network.
 * It takes a unsigned int which represents the position of the layer
 * in the Neural Network and returns a vector of node pointers
 * which are pointers to nodes in that specific layer.
 */
std::vector<Node*> & NeuralNetwork::getLayer(unsigned int pos)
{
    if (pos == 0)
        return inputs;
    else if (pos <= hiddenLayer.size())
        return hiddenLayer[pos - 1];

    return outputs;
}
