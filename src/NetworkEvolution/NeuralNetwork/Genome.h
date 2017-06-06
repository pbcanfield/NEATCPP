#include <vector>
#include <string>
#include "Gene.h"

class Genome
{
public:
	Genome();
	void addGene(Gene);
	void saveGenone(std::string);

	void loadFromFile(std::string);

	Gene getGene(unsigned int);


private:
	std::vector<Gene> geneticCode;
};
