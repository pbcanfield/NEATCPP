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

    // Calculating and Learning
    double const value();
    void backwardProp();

    // Modifying the network
    void removeFConnections();
    void removeBConnections();
    void addForward(Node *, Node *);
    void addBackwards(Weight *);

    // Getter
    Weight * getLastForward();
    Weight * getLastBackwards();

    // Utility functions
    double const sigDeriv();

private:
    std::vector<Weight*> fConnections;
    std::vector<Weight*> bConnections;

    double bias = 0;

};

#endif
