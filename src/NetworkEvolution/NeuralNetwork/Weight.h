/**
 * Author: Daiwei Chen
 * Date: 2017-6-11
 */

#ifndef WEIGHT_H
#define WEIGHT_H

#include "Node.h"

class Node;
class Weight
{
public:
    // Constructor and destructor
    Weight();
    Weight(Node *, Node *, double);
    ~Weight();

    // Acts as the getter and setter for the weight value
    double & value(){return weight;}

    // Getter and setter for the connections
    Node*& fNode() { return fConnection; };
    Node*& bNode() { return bConnection; };
    
private:
    // The connections themselves
    Node* fConnection;
    Node* bConnection;

    // The value of the weight
    double weight;
};

#endif
