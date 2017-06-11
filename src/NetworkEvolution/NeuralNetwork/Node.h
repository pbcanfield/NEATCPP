#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>

class Node
{
public:
    Node();
    ~Node();

    // Calculating and Learning
    double const value();
    void backwardProp();

    // Modifying the network
    void addForward(Node*);
    void addBackward(Node*);

    // Getters
    Node* getLastForward();
    Node* getLastBackward();

    // Utility functions
    double const sigDeriv();
    double& weight(){ return weigh; }

private:
    std::vector<Node*> fConnections;
    std::vector<Node*> bConnections;

    double weigh;
    double bias = 0;

};

#endif
