/*
 * Author: Pete Canfield & Daiwei Chen
 * Date: 2017-6-15
 */
#ifndef GENOME_H
#define GENOME_H

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "Gene.h"

class Genome
{
public:
	Genome();
	Genome(unsigned int, unsigned int);
	Genome(std::string);

    Genome cross(Genome);

    void addGene(Gene);
    void addBias(Bias);
	void setInput(std::vector<unsigned int>);
	void setHidden(std::vector<std::vector<unsigned int>>);
	void setOutput(std::vector<unsigned int>);

	void addInput(unsigned int);
	void addHidden(unsigned int,unsigned int);
	void insertHidden(unsigned int,unsigned int);
	void addOutput(unsigned int);

	void saveGenome(std::string);
	void loadFromFile(std::string);
    void copyIntoGenome(Genome &);
	void sortByGeneration();


    std::vector<Gene> getGeneVector();
    std::vector<Bias> getBiasVector();
	Gene & getGene(unsigned int);
    Gene & getGene(unsigned int, unsigned int);
    Bias & getBias(unsigned int);
	unsigned int getGenomeSize();
    unsigned int getBiasSize();
    unsigned int & lastNode();

	std::vector<unsigned int> getInput();
	std::vector<std::vector<unsigned int>> getHidden();
	std::vector<unsigned int> getOutput();


	bool geneExist(unsigned int, unsigned int);

    double randomNumber();

private:
	std::vector<Gene> geneticCode;
    std::vector<Bias> biasInfo;
	std::vector<unsigned int> input;
	std::vector<std::vector<unsigned int>> hiddenLayer;
	std::vector<unsigned int> output;

    unsigned int ln;
	const unsigned int INT = sizeof(int);
	const unsigned int DOUBLE = sizeof(double);
    const unsigned int BOOL = sizeof(bool);
};
#endif
