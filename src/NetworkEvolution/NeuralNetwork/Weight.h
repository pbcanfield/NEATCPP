#ifndef WEIGHT_H
#define WEIGHT_H

#include "Node.h"

class Node;
class Weight
{
public:
    Weight();
    ~Weight();

    void setWeight(double);
    double getWeight();
private:
    Node* fconnection;
    Node* bconnection;
    double weight;
};

#endif
