/**
 * Author: Daiwei Chen
 * Date: 2017-6-11
 */

#include "Node.h"

/* Constructs the node */
Node::Node()
{


}

/* Constructs a node that pre-sets the value and the bias,
 * otherwise, the bias will be a NULL ptr and the value will be 0
 */
Node::Node(double bias)
{
     b = bias;
}

/* Destroys every connection after */
Node::~Node()
{
    for(auto & weight : fConnections)
        delete weight;
}

/* This function takes in a forward node, backward node, and a weight to
 * set the new weight value. This function will create a new weight with
 * all the connections and values, and add the said weight to the back
 * of the fConnections vector.
 */
void Node::addForward(Node * forward, Node * back, double weight)
{
    fConnections.push_back(new Weight(forward, back, weight));
}

/* This function will take in a weight pointer and add the said weight onto
 * the bConnections vector.
 */
void Node::addBackwards(Weight * weight)
{
    bConnections.push_back(weight);
}

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

void Node::removeBackPointer(unsigned int pos)
{
    bConnections.erase(bConnections.begin() + pos);
}


/* This function is used to connect up the network.
 * Returns the last connection made forwards.
 */
Weight * Node::getLastForward()
{
    return fConnections.back();
}

/* This function is used to connect up the network.
 * Returns the last connection made forwards.
 */
Weight * Node::getLastBackwards()
{
    return bConnections.back();
}

/* This function calculates the value of *this* node based on its many connections.
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

void Node::backPropogation(double learningRate, double target)
{
    delta = (val - target) * (val * (1.0 - val));
    for(auto weight : bConnections)
        weight -> calculateGradient(delta,learningRate);

    if(b != 0)
        b -= learningRate * delta;
}

void Node::updateWeights()
{
    for(auto & weight : bConnections)
        weight -> update();
}


double Node::sigmoidActivation(double x)
{
    return 1.0 / (1 + pow(M_E,x * -1));
}

bool Node::isBiasEnabled()
{
    return b == 0 ? false:true;
}
