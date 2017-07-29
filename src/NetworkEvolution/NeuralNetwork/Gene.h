/**
 * Author: Pete Canfield
 * Date: 2017-7-28
 */

#ifndef GENE_H
#define GENE_H
/**
 * Genes describe the conenctions between nodes in a neural network.
 * They contain information about the nodes that they are between in the form
 * of node ID's and the weight they are storing. They also store a generation
 * number that tells when structure evolved.
 */
struct Gene
{
	unsigned int inID, outID;
	double weight;
	int generation;
};


/**
 * Biases describe the connections of biases in a neural network. If this is not
 * specified for a node in the neural network the bais for that node is 0 and
 * this does not have any effect on the sigmoid function.
 */
struct Bias
{
	double bias;
	unsigned int node;
	int generation;
};
#endif
