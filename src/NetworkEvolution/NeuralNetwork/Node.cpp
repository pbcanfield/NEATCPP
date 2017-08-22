/**
 * Author: Daiwei Chen
 * Date: 2017-6-11
 */
#include "Node.h"

/** Constructs the node */
Node::Node(unsigned int num)
{
    ID = num;
}

/**
 * Constructs a node that pre-sets the value and the bias,
 * otherwise, the bias will be a NULL ptr and the value will be 0
 */
Node::Node(double bias)
{
     b = bias;
}


/** Destroys every connection after */
Node::~Node()
{
    for(auto & weight : fConnections)
        delete weight;
}

/**
 * This function takes in a forward node, backward node, and a weight to
 * set the new weight value. This function will create a new weight with
 * all the connections and values, and add the said weight to the back
 * of the fConnections vector.
 */
void Node::addForward(Node * forward, Node * back, double weight)
{
    fConnections.push_back(new Weight(forward, back, weight));
}


/**
 * This function will take in a weight pointer and add the said weight onto
 * the bConnections vector.
 */
void Node::addBackwards(Weight * weight)
{
    bConnections.push_back(weight);
}

/**
 * This is a function that removes a connection from this node and also removes
 * the backwards connection from the corrisponding node it is coneccted to while
 * deleting the weight off the heap. nodeOut is a the output node that makes up
 * the connection that is being removed.
 */
void Node::removeConnection(Node * nodeOut)
{
    Node * testing;
    for(unsigned int i = 0; i < fConnections.size(); ++i)
    {
        testing = fConnections[i] -> fNode();
        if(testing == nodeOut)
        {
            for(unsigned int j = 0; j < testing -> getBackWeightSize(); ++j)
                if(testing -> getBackWeight(j) == fConnections[i])
                    testing -> removeBackPointer(j);

            delete fConnections[i];
            fConnections.erase(fConnections.begin() + i);
            return;
        }
    }
}

/**
 * This is a function that removes one of the backwards pointers in the node.
 * @param pos is used as an index for the position of the connection bering
 * removed.
 */
void Node::removeBackPointer(unsigned int pos)
{
    bConnections.erase(bConnections.begin() + pos);
}


/**
 * This function is used to connect up the network.
 * Returns the last connection made forwards.
 */
Weight * Node::getLastForward()
{
    return fConnections.back();
}

/**
 * This function is used to connect up the network.
 * Returns the last connection made backwards.
 */
Weight * Node::getLastBackwards()
{
    return bConnections.back();
}


/**
 * This function calculates the value of *this* node based on its many connections.
 * The value of the current node will be changed to the sum of all its connected
 * weights times the weight's connected node values.
 */
void Node::calculate()
{
    double sum = 0;
    for(auto weight : bConnections) {
        sum += weight->value() * weight->bNode()->value();
    }
    sum += b;

    val = sigmoidActivation(sum);
}


/**
 * This function is responsible for doing the calculations for the gradient
 * decent in the network. This particular function is for the case that gradient
 * decent is being calculated on a hidden node and does not need any other
 * parameters than the learning rate. It uses the values of previously connected
 * weights and the deltas to pass off critical values needed for the weights
 * to calculate the updates.
 * @param learningRate is a double value that specifies how big of a step will
 * be taken in the gradient decent process.
 */
void Node::backPropogation(double learningRate)
{

    double sum = 0;
    for(auto & weight : fConnections)
        sum += weight -> fNode() -> getDelta() * weight -> value();

    delta = sum * val * (1.0 - val);

    for(auto weight : bConnections)
        weight -> calculateGradient(delta,learningRate);

    if(b != 0)
        b -= learningRate * delta;
}


/**
 * This is a function that is similar to the one above except for one slight
 * variation in the way the delta is calculated. This function is called when
 * the updates to the weights connected to the output nodes are being optimized.
 * This case requires how far off the node is to its corrisponding training
 * value which is what the target is for.
 * @param learningRate is a double for describing how big of steps gradient
 * decent will take.
 * @param target is the target value the node is optimizing to.
 */
void Node::backPropogation(double learningRate, double target)
{
    delta = (val - target) * (val * (1.0 - val));
    for(auto weight : bConnections)
        weight -> calculateGradient(delta,learningRate);

    if(b != 0)
        b -= learningRate * delta;
}


/**
 * This is the function that applies the weight updates to all forward
 * connecting weights to this node.
 */
void Node::updateWeights()
{
    for(auto & weight : bConnections)
        weight -> update();
}

/**
 * This is the sigmoid activation function which is a logistic function that
 * squashes any input value to a number between 1 and 0.
 * @param  x is the input number in the function.
 * @return the value of sigmoid(x).
 */
double Node::sigmoidActivation(double x)
{
    return 1.0 / (1 + pow(M_E,x * -1));
}


/**
 * This is a function that checks if the bias is enabled by checking if the
 * value of the bias is 0.
 * @return if the value is 0 it will return false otherwise true is returned.
 */
bool Node::isBiasEnabled()
{
    return b == 0 ? false:true;
}
