#ifndef WEIGHT_H
#define WEIGHT_H

#include "Node.h"

class Node;
class Weight
{
 public:
  Weight();
  
 private:
  Node* fconnection;
  Node* bconnection;
  double weight;
};

#endif
