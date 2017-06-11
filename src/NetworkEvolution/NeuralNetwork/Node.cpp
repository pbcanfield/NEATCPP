#include "Node.h"
#include "Weight.h"

// Constructor
Node::Node()
{
    
}

Node::~Node()
{
    for(auto & connection : fConnections)
        delete connection;

    for(auto & connection : bConnections)
        delete connection;
}

void Node::addForward(Node * forward, Node * back)
{
    fConnections.push_back(new Weight(forward,back));
}

void Node::addBackwards(Weight * weight)
{
    bConnections.push_back(weight);
}

Weight * Node::getLastForward()
{
    return fConnections.back();
}

Weight * Node::getLastBackwards()
{
    return bConnections.back();
}
