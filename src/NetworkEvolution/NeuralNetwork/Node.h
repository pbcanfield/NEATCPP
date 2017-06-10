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

    // Expanding the network
    void addFConnections();
    void addBConnections();

    // Modifying the network
    void removeFConnections();
    void removeBConnections();

    // Getter
    std::vector<Weight*> const fConnections();
    std::vector<Weight*> const bConnections();
    
    // Utility functions
    double const sigDeriv();
  
private:
    std::vector<Weight*> fConnections;
    std::vector<Weight*> bConnections;

    double bias = 0;

};

#endif
