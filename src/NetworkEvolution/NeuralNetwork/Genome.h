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
	Genome(std::string);
	void addGene(Gene);
    void addBias(double);
	void setInput(unsigned int);
	void setHidden(std::vector<unsigned int>);
	void setOutput(unsigned int);

	void addInput();
	void addHidden(unsigned int);
	void addOutput();

	void saveGenome(std::string);
	void loadFromFile(std::string);
    void copyIntoGenome(Genome &);

    std::vector<Gene> getGenes();
    std::vector<double> getBiasVector();
	Gene getGene(unsigned int);
    double getBias(unsigned int);
	unsigned int getGenomeSize();
    unsigned int getBiasSize();

	unsigned int getInput();
	std::vector<unsigned int> getHidden();
	unsigned int getOutput();

private:

	std::vector<Gene> geneticCode;
    std::vector<double> biasInfo;
	unsigned int input;
	std::vector<unsigned int> hiddenLayer;
	unsigned int output;
};

#endif
