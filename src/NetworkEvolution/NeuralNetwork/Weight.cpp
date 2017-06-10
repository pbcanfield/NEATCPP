#include "Node.h"
#include "Weight.h"

Weight::Weight()
{

}

Weight::~Weight()
{
    delete fconnection;
    delete bconnection;
}

void Weight::setWeight(double value)
{
    weight = value;
}

double Weight::getWeight()
{
    return weight;
}
