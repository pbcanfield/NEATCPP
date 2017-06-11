#ifndef WEIGHT_H
#define WEIGHT_H

#include "Node.h"

class Node;
class Weight
{
public:
    Weight();
    Weight(Node *, Node *);
    ~Weight();

    double & value(){return weight;}
private:
    Node* fconnection;
    Node* bconnection;
    double weight;
};

#endif
