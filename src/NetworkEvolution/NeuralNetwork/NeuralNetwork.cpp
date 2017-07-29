/**
 * Author: Pete Canfield
 * Date: 2017-7-19
 */
#include "NeuralNetwork.h"


/**
 * This is the defualt constructor that sets the generation to zero
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

    if(isVisualized)
    {
        vThread -> join();
        delete vThread;
    }
    //Delete the genome.
    delete dna;

    //Delete the nodes of the network.
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
    unsigned int size = dna -> getInput();
    for(unsigned int i = 0; i < size; ++i)
        inputs.push_back(new Node());

    //Create the hidden layer.
    std::vector<unsigned int> topo = dna -> getHidden();
    size = topo.size();

    hiddenLayer.resize(size);

    for(unsigned int i = 0; i < size; ++i)
        for(unsigned int j = 0; j < topo[i]; ++j)
            hiddenLayer[i].push_back(new Node());


    //Create the output layer.
    size = dna -> getOutput();
    for(unsigned int i = 0; i < size; ++i)
        outputs.push_back(new Node());


    //Connect structure based on the genes in the genome and add the correct weights.
    unsigned int numConnections = dna -> getGenomeSize();
    Gene currentGene;
    for(unsigned int i = 0; i < numConnections; ++i)
    {
        currentGene = dna -> getGene(i);
        addWeight(findNodeWithID(currentGene.inID),findNodeWithID(currentGene.outID),
                                 currentGene.weight);
    }

    //Could optimize this bit but I dont know if its worth the memory overhead
    //to copy into a seperate vector.
    //sets the biases.
    for(auto & bias : dna -> getBiasVector())
        findNodeWithID(bias.node) -> bias() = bias.bias;

}


/**
 * The mutateAddWeight member function takes in two node ID's and creates a Weight
 * connection between them. When this happens the Genome of the NeuralNetwork
 * is updated and global network generation number is incramented.
 */
void NeuralNetwork::mutateAddWeight(unsigned int nodeOne, unsigned int nodeTwo)
{
    //Change 1 to a random weight later.
    Gene connection;
    connection.inID = nodeOne;
    connection.outID = nodeTwo;
    connection.weight = 1;
    connection.generation = generation;
    addWeight(findNodeWithID(nodeOne),findNodeWithID(nodeTwo),1);
    dna -> addGene(connection);
    ++generation;
}


/**
 * The mutateAddNode member function takes two node ID's and deletes the existing
 * connection and weight between these ID's. The function then decides if a layer
 * exists that the new intermediary node can be added to. If there is an existing
 * layer that the node can be added to the node is pushed back to that Node *
 * vector. Otherwise the member function inserts a new vector with one new Node *
 * where it is nedded. The function then ajusts the genome correctly and updates
 * the ID's that need to be altered in the Genome and inserts two new weights
 * that connect to the new node and the previous first and last node. The
 * generation number is then incramented.
 */
void NeuralNetwork::mutateAddNode(unsigned int nodeOne, unsigned int nodeTwo)
{

    unsigned int layer = findLayerFromNodeID(nodeOne);
    unsigned int layerDiff = findLayerFromNodeID(nodeTwo) - layer;
    if(layerDiff % 2 != 0)
    {
        std::vector<Node *> _layer;
        hiddenLayer.insert(hiddenLayer.begin() + layer,_layer);
        dna -> insertHidden(layer);
    }
    else
        dna -> addHidden(layer);

    Node * first = findNodeWithID(nodeOne);
    Node * last = findNodeWithID(nodeTwo);

    hiddenLayer[layer].push_back(new Node());
    Node * middle = hiddenLayer[layer].back();
    //Change 1.0 to a random number.

    double w1 = 1.0, w2 = 1.0;

    addWeight(first, middle, w1);
    addWeight(middle, last, w2);

    Gene connection;
    connection.generation = generation;
    unsigned int firstID = findIDWithNode(first);
    unsigned int medID = findIDWithNode(middle);
    unsigned int lastID = findIDWithNode(last);

    dna -> updateConnectionStructure(medID);
    dna -> removeConnection(firstID,lastID);

    connection.inID = firstID;
    connection.outID = medID;
    connection.weight = w1;
    dna -> addGene(connection);
    connection.inID = medID;
    connection.outID = lastID;
    connection.weight = w2;
    dna -> addGene(connection);
    ++generation;
}


/**
 * The mutateAddBias function adds a bias value to the node that corrilates
 * with the supplied node ID. The Genome is updated and then the generation
 * number is incramented.
 */
void NeuralNetwork::mutateAddBias(unsigned int node)
{
    Bias bias;
    //Again change 1 to a random number.
    double rBias = 1.0;
    bias.node = node;
    bias.bias = rBias;
    bias.generation = generation;
    dna -> addBias(bias);
    findNodeWithID(node) -> bias() = rBias;
    ++generation;
}


/**
 * This function takes a vector of double values and sets the input layer nodes
 * to the value of the input vector. If the input vector is not the size of the
 * input layer the function will output an error message and not set the input
 * layer to the input vector.
 */
void NeuralNetwork::setInputs(std::vector<double> input)
{
    if(input.size() == inputs.size())
    {
        unsigned int size = input.size();
        for(unsigned int i = 0; i < size; ++i)
            inputs[i] -> value() = input[i];
    }
    else
    {
        std::cout << "Could not set inputs becuase the input set was not the correct length"
            << std::endl;
    }
}


/**
 * This is the member function that sets the training vector of the Neural Network.
 * The function will only set the training vector if it matches the length of
 * the output layer.
 */
void NeuralNetwork::setTraining(std::vector<double> traingingVector)
{
    if(traingingVector.size() == outputs.size())
        training = traingingVector;
    else
        std::cout << "Could not set the training examples because the training set is not the correct length"
            << std::endl;
}


/**
 * This is the member function that is responisble for running
 * the single threaded forward propogation for the
 * neural network.
 */
void NeuralNetwork::runForward()
{
    //THANKS KRITIKA
    if (hiddenLayer.size() > 0)
    {
        for (auto & node : hiddenLayer[0])
            node -> calculate();

        unsigned int size = hiddenLayer.size();

        for(unsigned int i = 1; i < size; ++i)
            for(unsigned int j = 0; j < hiddenLayer[i].size(); ++j)
                hiddenLayer[i][j] -> calculate();
    }
    for (auto & node: outputs)
        node -> calculate();
}


/**
 * This is the function that is responisble for handling
 * the multithreading that runs the forward propogtion
 * for the Neural Network.
 */
void NeuralNetwork::runForward(unsigned int numThreads)
{
    ++numThreads;
    //Initialize completed to numThreads for first iteration.
    completed = 0;
    unsigned int totalLayers = hiddenLayer.size() + 2;


    //This is where the multithreading starts.
    for(unsigned int i = 0; i < numThreads - 1; ++i)
        threads.push_back(new std::thread(&NeuralNetwork::processForward,this,i,numThreads));

    unsigned int thisLayer = 1;
    unsigned int rem;

    while(thisLayer != totalLayers)
    {
        ++completed;

        lockFunc(completed,numThreads);

        completed = 0;
        mLock.lock();
        unsigned int nLayer = findNumInLayer(thisLayer);
        mLock.unlock();

        rem = nLayer % numThreads;
        if(rem != 0)
        {
            mLock.lock();
            std::vector<Node*> & currentLayer = getLayer(thisLayer);
            mLock.unlock();

            for(unsigned int i = nLayer - rem; i < nLayer; ++i)
                currentLayer[i] -> calculate();
        }
        ++thisLayer;
    }


    for(auto & thread : threads)
    {
        thread -> join();
        delete thread;
    }

}


/**
 * This is a member function that returns the least mean squared error of the
 * entire Neural Network. It is calculated by taking the sum of the square of the
 * difference between the expected and the output across the entre training and
 * output vector.
 */
double NeuralNetwork::getLMSError()
{


    double sum = 0;
    if(training.size() > 0)
    {
        unsigned int size = outputs.size();
        for (unsigned int i = 0; i < size; ++i)
            sum += 0.5 * pow(training[i] - outputs[i] -> value(),2);
    }
    else
        std::cout << "The examples have not been set, LMS can't be calculated"
                  << std::endl;
    return sum;
}


/**
 * This is the algorithm that performs the gradient decent caluclations for
 * backPropogation. All the math such as calculating the node deltas is handled
 * by the Node class. This member function simply calls the corrisponding
 * backPropogation function for each node. After the new weights are all
 * calculated they are all applied after all gradient decent caluclations have
 * been made.
 */
void NeuralNetwork::gradientDecent(double learningRate)
{
    unsigned int size = outputs.size();
    for(unsigned int i = 0; i < size; ++i)
        outputs[i] -> backPropogation(learningRate,training[i]);

    for(int i = hiddenLayer.size() - 1; i >= 0; --i)
        for(unsigned int j = 0; j < hiddenLayer[i].size(); ++j)
            hiddenLayer[i][j] -> backPropogation(learningRate);

    for(auto & node : outputs)
        node -> updateWeights();

    for(int i = hiddenLayer.size() - 1; i >= 0; --i)
        for(unsigned int j = 0; j < hiddenLayer[i].size(); ++j)
            hiddenLayer[i][j] -> updateWeights();

}


/**
 * This function saves the current genome to a charzar file.
 * It takes a string that is the directory of the file and the
 * charzar extension is automatically attatched to the file.
 */
void NeuralNetwork::saveNetwork(std::string name)
{
    if(name.find(".charzar") == std::string::npos)
        name += ".charzar"; //maybe dont do it this way


    //Update Genes and Biases in the Genome.
    unsigned int gCount = 0, bCount = 0;
    for(auto & node : inputs)
    {
        updateGene(node,gCount);
        updateBias(node,bCount);
    }
    for(auto & vec : hiddenLayer)
    {
        for(auto & node : vec)
        {
            updateGene(node,gCount);
            updateBias(node,bCount);
        }
    }
    for(auto & node : outputs)
        updateBias(node,bCount);

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
 * This member function copies the values of the output nodes into a vector that
 * is then returned. It is used to get the raw output of the network.
 */
std::vector<double> NeuralNetwork::getNetworkOutput()
{
    std::vector<double> value;
    for(auto & node : outputs)
        value.push_back(node -> value());

    return value;
}


/**
 * This is the Visualization function that currently (may be changed later)
 * generates a simple visualization of the neural network that is not in real
 * time. It would be easy enough to make the visualization update in real time
 * it was not implamented this way in fear of data races and for convienence.
 * Does not work on OS X due to a limition of the OS and SFML.
 */
void NeuralNetwork::visualize(unsigned int x, unsigned int y)
{
    isVisualized = true;
    vThread = new std::thread(&NeuralNetwork::displayWindow,this,*dna,x,y);
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
 * This takes a Node * and returns the current node ID based on its placement in
 * the network. It could probably be optimized which would leed to much better
 * speed.
 */
unsigned int NeuralNetwork::findIDWithNode(Node * target)
{
    bool found = false;
    unsigned int ID;
    unsigned int size = inputs.size();
    unsigned int count = size;

    for(unsigned int i = 0; i < size && !found; ++i)
    {
        if (inputs[i] == target)
        {
            found = true;
            ID = i;
        }
    }
    size = hiddenLayer.size();
    for(unsigned int i = 0; i < size && !found; ++i)
    {
        for(unsigned int j = 0; j < hiddenLayer[i].size() && !found; ++j)
        {
            if(hiddenLayer[i][j] == target)
            {
                found = true;
                ID = count;
            }
            ++count;
        }
    }
    size = outputs.size();
    for(unsigned int i = 0; i < size && !found; ++i)
    {
        if(outputs[i] == target)
        {
            found = true;
            ID = count;
        }
    }
    return ID;
}


/**
 * This is a function that takes a thread Id and then calculates and runs the specific
 * forward propogation calculations that are neccissary for that thread to complete for
 * the specific layer. The layer that is currently being processed is managed by the
 * main thread in the runForward function.
 * It takes an unsinged int as an input which represents which thread is running this function.
 */
void NeuralNetwork::processForward(unsigned int ID, unsigned int numThreads)
{
    unsigned int workload, numInLayer;
    unsigned int thisLayer = 1;

    std::vector<Node*> layer;

    mLock.lock();
    unsigned int totalLayers = hiddenLayer.size() + 2;
    mLock.unlock();

    while(thisLayer != totalLayers)
    {
        ++completed;


        lockFunc(completed,numThreads);
        lockFunc(completed,0);

        mLock.lock();
        layer = getLayer(thisLayer);
        numInLayer = findNumInLayer(thisLayer);
        mLock.unlock();

        workload = ((numInLayer - ( numInLayer % numThreads)) / numThreads) + ID;
        for(unsigned int i = ID; i < workload; ++i)
            layer[i] -> calculate();

        ++thisLayer;

    }
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


/**
 * This function locks a specific thread until a target variable reaches a
 * a certain value.
 */
void NeuralNetwork::lockFunc(std::atomic<unsigned int> & current, unsigned int target)
{
    bool go = false;
    while (!go)
    {
        if(current == target)
            go = true;
        else
            std::this_thread::yield();
    }
}


/**
 * This is the helper function that actually creates and displays the visualization
 * of This neural network. This is not done yet as I couldnt program it on OS X
 * do to a OS limition.
 */
void NeuralNetwork::displayWindow(Genome code, unsigned int winX, unsigned int winY)
{
    sf::RenderWindow window(sf::VideoMode(winX,winY),"Neural Network Visualization");


    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.display();
    }
}


/**
 * This function updates a Gene within the Genome of the Neural Network and
 * is called from the saveNetwork function.
 */
void NeuralNetwork::updateGene(Node * node, unsigned int & gCount)
{
    unsigned int size = node -> getForwardSize();
    for(unsigned int i = 0; i < size; ++i)
    {
        dna -> getGene(gCount).weight = node -> getFrowardWeight(i) -> value();
        ++gCount;
    }
}


/**
 * This function updates a Bias within the Genome of the Neural Network and
 * is called from the saveNetwork function.
 */
void NeuralNetwork::updateBias(Node * node, unsigned int & bCount)
{
    if(node -> isBiasEnabled())
    {
        dna -> getBias(bCount).bias = node -> bias();
        ++bCount;
    }
}


/**
 * This is a helper function that adds a weight and connection between two nodes.
 * It takes a first node pointer and a second node pointer and a weight value.
 */
void NeuralNetwork::addWeight(Node * first, Node * last, double weight)
{
    first -> addForward(last,first,weight);
    last -> addBackwards(first -> getLastForward());
}


/**
 * This is a helper funtion that searches the topology of the network ad returns
 * the layer number that a node belongs to. It takes an input node ID and returns
 * a layer number.
 */
unsigned int NeuralNetwork::findLayerFromNodeID(unsigned int ID)
{
    unsigned int nInput = inputs.size();
    unsigned int hSize = hiddenLayer.size();


    if(ID < nInput)
    {
        return 0;
    }
    else
    {
        unsigned int sum = nInput;
        for(unsigned int i = 0; i < hSize; ++i)
        {
            sum += hiddenLayer[i].size();
            if(ID < sum)
                return i + 1;

        }
    }
    return hSize + 1;
}
