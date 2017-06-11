#include "Node.h"
#include "Weight.h"

Weight::Weight()
{

}
Weight::Weight(Node * forward, Node * backwards, double w)
{
    fConnection = forward;
    bConnection = backwards;
    weight = w;
}

Weight::~Weight()
{
    //This is a weird situation it might not need to be freed here.
    //delete fconnection;
    //delete bconnection;
}
