#pragma once

#include <algorithm>
#include <map>

#include "Chromosome.hpp"

#define POPULATION_SIZE 100

class Population
{
public:
	Population(int numberOfGenes = POPULATION_SIZE, std::vector<Gene> = std::vector<Gene>());

	void create();

	double overallFitness() const;
	void makeSelectionProbabilities();
	void makePreviousProbabilities();

	Chromosome fittestIndividual();

	int selectFittestParentIndex(int parentIndex = -1);
	int selectParentIndex(double, int parentIndex = -1);

	int selectUnfitIndividual(int otherUnfitIndividual = -1);

	Chromosome getAt(int);
	void update(int, Chromosome);

	void print() const;

private:
	int numberOfGenes;
	std::map<int, Chromosome> population;
	std::vector<double> individualProbabilities;
	std::vector<double> previousProbabilities;
	std::vector<Gene> genesCoordinates;

	void createPopulation();

};

