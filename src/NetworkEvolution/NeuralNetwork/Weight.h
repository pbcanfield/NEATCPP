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

    void calculateGradient(double,double);
    void update();

    Node *& fNode() {return fConnection;}
    Node *& bNode() {return bConnection;}
    double & value(){return weight;}


private:
    Node* fConnection;
    Node* bConnection;
    double weight,updatedWeight;
};

#endif
