/**
 * Author: Daiwei Chen
 * Date: 2017-6-11
 */

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>
#include <math.h>
#include <cstddef>
#include "Weight.h"

class Node
{
public:
    // Constructor and Destructor
    Node();
    Node(double);
    ~Node();

    // Calculating and Learning
    void calculate();
    void backPropogation(double);
    void backPropogation(double,double);
    void updateWeights();


    // Modifying the network
    void removeFConnections();
    void removeBConnections();
    void addForward(Node *, Node *, double=0);
    void addBackwards(Weight *);
    void setBiasPtr(double *);

    // Getter
    Weight * getLastForward();
    Weight * getLastBackwards();
    double & value() { return val; }
    double * getBiasPtr() { return bias; }
    double getOutDerivative() { return outDer; }
    double getETotal() { return eTotal; }
private:

    double sigmoidDerivative(double);
    double sigmoidActivation(double);

    // These are all the connections of the nodes, the weights will act as a middleman
    // for the node, since
    std::vector<Weight*> fConnections;
    std::vector<Weight*> bConnections;

    // The value and sum
    double val,eTotal,outDer;


    //This is the bias value for the network.
    double * bias = NULL;

};

#endif
