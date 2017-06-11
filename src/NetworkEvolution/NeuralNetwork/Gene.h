#ifndef GENE_H
#define GENE_H

struct Gene
{
	int inID, outID;
	double weight;
	bool enabled;
	int generation;
};

#endif
