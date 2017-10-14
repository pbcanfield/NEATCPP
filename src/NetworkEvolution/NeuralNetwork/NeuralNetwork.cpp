/**
 * Author: Pete Canfield
 * Date: 2017-7-19
 */
#include "NeuralNetwork.h"


/**
 * This is the defualt constructor that sets the generation to zero
 * and allocates the memory for the genome of this Neural Network.
 */
NeuralNetwork::NeuralNetwork(unsigned int seed)
{
    srand(seed);
    generation = 0;
    dna = new Genome();
}


/**
 * This is a constructor that takes a string that will automatically
 * load the genome from a specified charzar file and updates structure
 * accordingly.
 */
NeuralNetwork::NeuralNetwork(std::string dir, unsigned int seed)
{
    srand(seed);
    dna = new Genome();
    generation = 0;


    dna -> loadFromFile(dir);
    updateStructure();

}


/**
 * This is a constructor that takes a genome and copies that genome
 * into this networks genome and updates the structure accordingly.
 */
NeuralNetwork::NeuralNetwork(Genome code, unsigned int seed)
{
    srand(seed);
    dna = new Genome();
    generation = 0;

    dna -> copyIntoGenome(code);
    updateStructure();
}


/**
 * This is the function that is resposible for crossing Neural Networks
 * by crossing the genomes of the NeuralNetworks and returning a new
 * network pointer.
 */
NeuralNetwork * NeuralNetwork::operator+(NeuralNetwork & one)
{
    NeuralNetwork * networkPtr;
    return networkPtr;
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
    std::vector<unsigned int> _vec = dna -> getInput();
    for(auto & node : _vec)
        inputs.push_back(new Node(node));

    //Create the hidden layer.
    std::vector<std::vector<unsigned int>> topo = dna -> getHidden();
    unsigned int size = topo.size();

    hiddenLayer.resize(size);

    for(unsigned int i = 0; i < size; ++i)
        for(unsigned int j = 0; j < topo[i].size(); ++j)
            hiddenLayer[i].push_back(new Node(topo[i][j]));

    //Create the output layer.
    _vec = dna -> getOutput();
    for(auto & node : _vec)
        outputs.push_back(new Node(node));

    //Connect structure based on the genes in the genome and add the correct weights.
    unsigned int numConnections = dna -> getGenomeSize();
    Gene currentGene;
    for(unsigned int i = 0; i < numConnections; ++i)
    {
        currentGene = dna -> getGene(i);
        if(currentGene.enabled)
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
    double weight = dna -> randomNumber();
    Gene connection;
    connection.inID = nodeOne;
    connection.outID = nodeTwo;
    connection.weight = weight;
    connection.generation = generation;
    addWeight(findNodeWithID(nodeOne),findNodeWithID(nodeTwo),weight);
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
    unsigned int nodeID = ++dna -> lastNode();

    if(layerDiff % 2 != 0)
    {
        std::vector<Node *> _layer;
        hiddenLayer.insert(hiddenLayer.begin() + layer,_layer);
        dna -> insertHidden(layer,nodeID);
    }
    else
        dna -> addHidden(layer,nodeID);

    Node * first = findNodeWithID(nodeOne);
    Node * last = findNodeWithID(nodeTwo);

    hiddenLayer[layer].push_back(new Node(nodeID));
    Node * middle = hiddenLayer[layer].back();

    double w1 = dna -> randomNumber(), w2 = dna -> randomNumber();

    addWeight(first, middle, w1);
    addWeight(middle, last, w2);

    Gene connection;
    connection.generation = generation;
    unsigned int firstID = first -> getID();
    unsigned int lastID = last -> getID();

    dna -> getGene(firstID,lastID).enabled = false;

    connection.inID = firstID;
    connection.outID = nodeID;
    connection.weight = w1;
    dna -> addGene(connection);
    connection.inID = nodeID;
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
    double rBias = dna -> randomNumber();
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
void NeuralNetwork::visualize(unsigned int x, unsigned int y, unsigned int update)
{
    isVisualized = true;
    std::cout << "Launching window: press escape to exit" << std::endl;
    vThread = new std::thread(&NeuralNetwork::displayWindow,this,x,y,update);

}


/**
 * This is a function that takes a linear identification number and searches
 * the non-linear network structure for the corrisponding node.
 * It takes an unsigned in wich is an ID and returns a node pointer
 * which is the pointer to that specific node in the network structure.
 */
Node * NeuralNetwork::findNodeWithID(unsigned int ID)
{
    Node * nodePtr = NULL;
    Node * _ptr;

    //Search the input layer.
    unsigned int size = inputs.size();
    bool found = false;

    for(unsigned int i = 0; i < size && !found; ++i)
    {
        _ptr = inputs[i];
        if(_ptr -> getID() == ID)
        {
            nodePtr = _ptr;
            found = true;
        }
    }

    //Search the hidden layer.
    size = hiddenLayer.size();
    for(unsigned int i = 0; i < size && !found; ++i)
    {
        for(unsigned int j = 0; j < hiddenLayer[i].size() && !found; ++j)
        {
            _ptr = hiddenLayer[i][j];
            if(_ptr -> getID() == ID)
            {
                nodePtr = _ptr;
                found = true;
            }
        }
    }

    //Search the output layer.

    size = outputs.size();
    for(unsigned int i = 0; i < size && !found; ++i)
    {
        _ptr = outputs[i];
        if(_ptr -> getID() == ID)
        {
            nodePtr = _ptr;
            found = true;
        }
    }
    return nodePtr;
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
 * of This neural network.
 */
void NeuralNetwork::displayWindow(unsigned int winX, unsigned int winY,unsigned int updateRate)
{
    sf::RenderWindow window(sf::VideoMode(winX,winY),"Neural Network Visualization");
    unsigned int  frameRate = 60;

    unsigned int limit = updateRate * frameRate;
    unsigned int count = 0;

    float xDistance,yDistance;

    window.setFramerateLimit(frameRate);

    std::vector<sf::CircleShape> nodes;
    std::vector<sf::CircleShape> biases;
    std::vector<sf::Vertex> weights;
    std::vector<double> colorMultipliers;
    sf::CircleShape _shape;
    sf::Color nodeColor;
    nodeColor.g = 0;
    nodeColor.b = 200;


    while(window.isOpen())
    {

        if(count == 0)
        {
            //update the visualization
            //Optimize this later.
            nodes.clear();
            weights.clear();
            colorMultipliers.clear();

            float radius = 10;
            float bXOffset = 100;

            _shape.setOrigin(radius,radius);
            _shape.setRadius(radius);

            unsigned int numLayers = dna -> getHidden().size() + 2;


            mLock.lock();
            unsigned int numInput = dna -> getInput().size();


            std::vector<unsigned int> topo;

            {
            std::vector<std::vector<unsigned int>> _topo = dna -> getHidden();
            for(unsigned int i = 0; i < _topo.size(); ++i)
                topo.push_back(_topo[i].size());

            }

            unsigned int numOutput = dna -> getOutput().size();
            unsigned int size = dna -> getGenomeSize();
            std::vector<Bias> biasInformation = dna -> getBiasVector();

            for(auto & node : inputs)
                colorMultipliers.push_back(node -> value());

            for(auto & vec : hiddenLayer)
                for(auto & node : vec)
                    colorMultipliers.push_back(node -> value());

            for(auto & node : outputs)
                colorMultipliers.push_back(node -> value());

            mLock.unlock();

            unsigned int maxNum = numInput;
            for(unsigned int i = 0; i < topo.size(); ++i)
                if(topo[i] > maxNum)
                    maxNum = topo[i];
            if(numOutput > maxNum)
                maxNum = numOutput;


            float biasYDistance = calcDistance(winY,maxNum,radius) / 2.f;


            xDistance = calcDistance(winX,numLayers,radius);
            yDistance = calcDistance(winY,numInput,radius);
            //Calculate the positions of the nodes.
            _shape.setPosition(xDistance, yDistance + radius);
            nodeColor.r = 255 * colorMultipliers[0];
            _shape.setFillColor(nodeColor);
            nodes.push_back(_shape);


            for(unsigned int i = 1; i < numInput; ++i)
            {
                nodeColor.r = 255 * colorMultipliers[i];
                _shape.setFillColor(nodeColor);
                _shape.setPosition(xDistance, (i + 1) * yDistance + 2 * i * radius);
                nodes.push_back(_shape);
            }

            unsigned int count = numInput;
            for(unsigned int i = 0; i < topo.size(); ++i)
            {
                for(unsigned int j = 0; j < topo[i]; ++j)
                {
                    yDistance = calcDistance(winY,topo[i],radius);
                    nodeColor.r = 255 * colorMultipliers[count];
                    _shape.setFillColor(nodeColor);

                    if (j == 0) // Have to do it this way in case there are no hidden nodes.
                    {
                        _shape.setPosition(2 * xDistance + i * xDistance,
                                           yDistance + radius);
                        nodes.push_back(_shape);
                    }
                    else
                    {
                        _shape.setPosition(2 * xDistance + i * xDistance,
                                          (j + 1) * yDistance + 2 * j * radius);
                        nodes.push_back(_shape);
                    }
                    ++count;
                }
            }

            yDistance = calcDistance(winY,numOutput,radius);

            nodeColor.r = 255 * colorMultipliers[count];
            _shape.setFillColor(nodeColor);
            _shape.setPosition(xDistance * numLayers, yDistance + radius);
            nodes.push_back(_shape);

            ++count;

            for(unsigned int i = 1; i < numOutput; ++i)
            {
                nodeColor.r = 255 * colorMultipliers[count];;
                _shape.setFillColor(nodeColor);
                _shape.setPosition(xDistance * numLayers, (i + 1) * yDistance + 2 * i * radius);
                nodes.push_back(_shape);
                ++count;
            }

            sf::Vector2f pos;
            //Biases
            nodeColor.r = 255;
            _shape.setFillColor(nodeColor);
            std::vector<unsigned int> usedLayers;
            unsigned int _layer;
            bool found;
            for(auto & bias : biasInformation)
            {

                found = false;
                mLock.lock();
                _layer = findLayerFromNodeID(bias.node);
                mLock.unlock();
                for(auto layer : usedLayers)
                    if(layer == _layer)
                        found = true;

                mLock.lock();
                pos = nodes[bias.node].getPosition();
                weights.push_back(pos);
                pos = nodes[getLayer(_layer)[0] -> getID()].getPosition();
                pos.x += bXOffset;
                pos.y = biasYDistance;
                mLock.unlock();
                weights.push_back(pos);


                if(!found)
                {
                    usedLayers.push_back(_layer);
                    _shape.setPosition(pos);
                    biases.push_back(_shape);
                }
            }


            Gene _connection;

            //Add the lines.
            for(unsigned int i = 0; i < size; ++i)
            {
                mLock.lock();
                _connection = dna -> getGene(i);
                mLock.unlock();
                pos = nodes[_connection.inID].getPosition();
                weights.push_back(pos);
                pos = nodes[_connection.outID].getPosition();
                weights.push_back(pos);
            }




        }
        if (count < limit)
            ++count;
        else
            count = 0;

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    std::cout << "Exiting visualization" << std::endl;
                    window.close();
                }
            }

        }

        window.clear();


        window.draw(&weights[0],weights.size(),sf::Lines);

        for(auto & bias : biases)
            window.draw(bias);

        for(auto & shape : nodes)
            window.draw(shape);

        window.display();
    }
}


/**
 * This is a worker function that caluclates the distance between either nodes or
 * layers of nodes.
 * @param  max    Either the x or y size of the window.
 * @param  num    Either the number of layers or nodes
 *                in the network.
 * @param  radius The radius of the node.
 * @return        The distance between either nodes or layers.
 */
float NeuralNetwork::calcDistance(unsigned int max, unsigned int num, float radius)
{
    return (max - 2.f * num * radius) / (num + 1);
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
    unsigned int layer;
    bool found = false;
    unsigned int size = inputs.size();

    for(unsigned int i = 0; i < size && !found; ++i)
    {
        if(inputs[i] -> getID() == ID)
        {
            layer = 0;
            found = true;
        }
    }

    size = hiddenLayer.size();
    for(unsigned int i = 0; i < size; ++i)
    {
        for(unsigned int j = 0; j < hiddenLayer[i].size() && !found; ++j)
        {
            if(hiddenLayer[i][j] -> getID() == ID)
            {
                layer = i + 1;
                found = true;
            }
        }
    }

    size = outputs.size();
    for(unsigned int i = 0; i < size && !found; ++i)
    {
        if(outputs[i] -> getID() == ID)
        {
            layer = hiddenLayer.size() + 1;
            found = true;
        }
    }

    return layer;

}
