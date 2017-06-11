/**
 * Author: Daiwei Chen
 * Date: 2017-6-11
 */

#include "Node.h"
#include "Weight.h"

Weight::Weight()
{

}

Weight::Weight(Node * forward, Node * backward, double w)
    : fConnection(forward), bConnection(backward), weight(w)
{

}

Weight::~Weight()
{
    //This is a weird situation it might not need to be freed here.
    //delete fconnection;
    //delete bconnection;
}

