#include "Node.h"

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

void Node::addForward(Node* node)
{
    fConnections.push_back(node));
}

void Node::addBackward(Node* node)
{
    bConnections.push_back(node);
}

Node* Node::getLastForward()
{
    return fConnections.back();
}

Node* Node::getLastBackward()
{
    return bConnections.back();
}
