#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>
#include "Weight.h"

class Node
{
public:
    Node();
    ~Node();

    void forwardProp();
    void backwardProp();
    void addForward();
    void addBackwards(Weight *);

    Weight * getLastForward();
    Weight * getLastBackwards();

    double sigDeriv();
    double value();

private:
    std::vector<Weight*> fConnections;
    std::vector<Weight*> bConnections;

    double bias;

};

#endif
