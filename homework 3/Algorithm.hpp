#pragma once

#include "Population.hpp"

#define MAX_NO_IMPROVEMENT_ITERATIONS 10

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(int numberOfGenes = POPULATION_SIZE, std::vector<Gene> coordinates = std::vector<Gene>());

	Population getPopulation() const;

	void reproduction();

private:
	Population population;
	int numberOfGenes;
public:
	std::pair<Chromosome, Chromosome> selection();

	std::pair<Chromosome, Chromosome> crossover(std::pair<Chromosome, Chromosome>);

	void crossoverSwitchingParents(std::vector<Gene>&, std::vector<Gene>, int, std::set<Gene>);

	void mutation(Chromosome&);

	void updatePopulation(std::pair<Chromosome, Chromosome>);
};