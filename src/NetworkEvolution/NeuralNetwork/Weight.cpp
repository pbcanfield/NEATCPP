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

double& Weight::weight()
{
    return weight;
}
