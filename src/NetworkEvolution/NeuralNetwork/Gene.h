#ifndef GENE_H
#define GENE_H

struct Gene
{
	int inID, outID;
	double weight;
	int generation;
};

struct Bias
{
	double bias;
	unsigned int node;
};
#endif
