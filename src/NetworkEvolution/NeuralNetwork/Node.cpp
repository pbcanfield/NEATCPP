/**
 * Author: Daiwei Chen
<<<<<<< HEAD
 * Date: 2017-6-10
=======
 * Date: 2017-6-11
>>>>>>> 79db036014b8bb12af0064f852e3106b33dc8e8c
 */

#include "Node.h"

/* Constructs the node */
Node::Node()
{

<<<<<<< HEAD
=======
}

/* Constructs a node that pre-sets the value and the bias,
 * otherwise, both values will be 0 */
Node::Node(double v, double b)
    : val(v), bias(b)
{
    
>>>>>>> 79db036014b8bb12af0064f852e3106b33dc8e8c
}

/* Destroys every connection after */ 
Node::~Node()
{
<<<<<<< HEAD
    
}

void Node::addForward(Node* node)
{
    fConnections.push_back(node);
}

void Node::addBackward(Node* node)
=======
    for(auto & connection : fConnections)
        delete connection;
}

/* This function takes in a forward node, backward node, and a weight to 
 * set the new weight value. This function will create a new weight with
 * all the connections and values, and add the said weight to the back 
 * of the fConnections vector.
 */
void Node::addForward(Node * forward, Node * back, double weight = 0)
{
    fConnections.push_back(new Weight(forward, back, weight));
}

/* This function will take in a weight pointer and add the said weight onto
 * the bConnections vector.
 */
void Node::addBackwards(Weight * weight)
>>>>>>> 79db036014b8bb12af0064f852e3106b33dc8e8c
{
    bConnections.push_back(node);
}

<<<<<<< HEAD
Node* Node::getLastForward()
=======
/* This function is used to connect up the network.
 * Returns the last connection made forwards.
 */
Weight * Node::getLastForward()
>>>>>>> 79db036014b8bb12af0064f852e3106b33dc8e8c
{
    return fConnections.back();
}

<<<<<<< HEAD
Node* Node::getLastBackward()
=======
/* This function is used to connect up the network.
 * Returns the last connection made forwards.
 */
Weight * Node::getLastBackwards()
>>>>>>> 79db036014b8bb12af0064f852e3106b33dc8e8c
{
    return bConnections.back();
}

/* This function calculates the value of *this* node based on its many connections.
 * The value of the current node will be changed to the sum of all its connected 
 * weights times the weight's connected node values.
 */
void calculate()
{
    double sum;
    for(auto weight : bConnections) {
        sum += weight->value() * weight->bNode()->value();
    }
    val = sum + bias;
}

/* This function changes the bias. */
void setBias(double b)
{
    bias = b;
}
