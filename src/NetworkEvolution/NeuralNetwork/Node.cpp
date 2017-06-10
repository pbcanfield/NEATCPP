#include "Node.h"
#include "Weight.h"

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

void Node::addForward()
{
    fConnections.push_back(new Weight());
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
