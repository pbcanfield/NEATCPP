#ifndef GENE_H
#define GENE_H

struct Gene
{
	int inID, outID;
	double weight;
	bool enabled;
	int generation;
};

struct Bias
{
	double b;
	unsigned int layer;
};
#endif
