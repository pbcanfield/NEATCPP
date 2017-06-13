/**
 * Author: Daiwei Chen
 * Date: 2017-6-11
 */

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>
#include "Weight.h"

class Node
{
public:
    // Constructor and Destructor
    Node();
    Node(double, double);
    ~Node();

    // Calculating and Learning
    void calculate();

    // Modifying the network
    void removeFConnections();
    void removeBConnections();
    void addForward(Node *, Node *, double=0);
    void addBackwards(Weight *);
    void setBias(double);

    // Getter
    Weight * getLastForward();
    Weight * getLastBackwards();
    double const value() { return val; };


private:

    // These are all the connections of the nodes, the weights will act as a middleman
    // for the node, since
    std::vector<Weight*> fConnections;
    std::vector<Weight*> bConnections;

    // The value and the bias
    double val = 0;
    double bias = 0;

};

#endif
