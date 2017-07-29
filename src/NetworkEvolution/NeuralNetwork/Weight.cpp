/**
 * Author: Pete Canfield & Daiwei Chen
 * Date: 2017-7-28
 */
#include "Node.h"
#include "Weight.h"

/**
 * This is the default constructor that crates an empty weight.
 */
Weight::Weight()
{

}


/**
 * This is the constructor that somehow (ask Dave) initializes the forward abd
 * backwards connections in the neural network.
 * @param forward  The first node it is connected to.
 * @param backward The second node it is connected to.
 * @param w        The value of this weight.
 */
Weight::Weight(Node * forward, Node * backward, double w)
    : fConnection(forward), bConnection(backward), weight(w)
{

}

/**
 * This is the deconstructor for the weight. The fConnection and the bConnection
 * do not have to be freed here as they are freed when the NeuralNetwork's
 * deconstructor is called.
 */
Weight::~Weight()
{

}


/**
 * This is a member function that calculates the new value of the weight by
 * applying a node delta that is calculated by the corrisponding node and
 * by using the supplied learning rate.
 * @param delta        This is a previously calculated value from each node.
 * @param learningRate How big is each step of gradient decent going to be.
 */
void Weight::calculateGradient(double delta, double learningRate)
{
    updatedWeight = weight - (learningRate *
                             bConnection -> value() *
                             delta);
}

/**
 * This function copies the value of updatedWeight into the corrisponding weight
 * value and is called when all weight updates are being applied after gradient
 * decent.
 */
void Weight::update()
{
    weight = updatedWeight;
}
