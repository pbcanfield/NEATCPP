#ifndef WEIGHT_H
#define WEIGHT_H

#include "Node.h"

class Node;
class Weight
{
public:
    Weight();
    Weight(Node *, Node *, double = 0);
    ~Weight();
    Node *& fNode() {return fConnection;}
    Node *& bNode() {return bConnection;}
    double & value(){return weight;}
private:
    Node* fConnection;
    Node* bConnection;
    double weight;
};

#endif
