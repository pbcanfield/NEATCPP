#ifndef WEIGHT_H
#define WEIGHT_H

#include "Node.h"

class Node;
class Weight
{
public:
    Weight(Node *, Node *);
    ~Weight();

    double & weight();
private:
    Node* fconnection;
    Node* bconnection;
    double val;
};

#endif
