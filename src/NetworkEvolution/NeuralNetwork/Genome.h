#ifndef GENOME_H
#define GENOME_H

#include <vector>
#include <string>
#include <fstream>
#include "Gene.h"

class Genome
{
public:
	Genome();
	void addGene(Gene);
	void setInput(unsigned int);
	void setHidden(std::vector<unsigned int>);
	void setOutput(unsigned int);

	void addInput();
	void addHidden(unsigned int);
	void addOutput();

	void saveGenome(std::string);
	void loadFromFile(std::string);

	Gene getGene(unsigned int);
	unsigned int getGenomeSize();

	unsigned int getInput();
	std::vector<unsigned int> getHidden();
	unsigned int getOutput();

private:

	std::vector<Gene> geneticCode;

	unsigned int input;
	std::vector<unsigned int> hiddenLayer;
	unsigned int output;
};

#endif
