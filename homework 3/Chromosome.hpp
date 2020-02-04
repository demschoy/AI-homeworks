#pragma once

#include <vector>
#include <set>
#include <random>
#include <cmath>

#include "Gene.hpp"

class Chromosome
{
public:
	Chromosome();
	Chromosome(int, std::vector<Gene>);
	Chromosome(std::vector<Gene>);

	void create();

	int getNumberOfGenes() const;
	std::vector<Gene> getIndividual() const;
	void mutate(std::vector<Gene>);

	double distance() const;
	double fitness() const;
	double calculateSelectionProbability(double) const;

	void print() const;

	friend bool operator<(const Chromosome& left, const Chromosome& right);

private:
	int numberOfGenes;
	std::vector<Gene> individual;
	std::vector<Gene> coordinates;

	void createIndividual(std::vector<Gene>);
};