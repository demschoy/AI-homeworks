#include "Population.hpp"

Population::Population(int _numberOfGenes, std::vector<Gene> coordinates)
{
	numberOfGenes = _numberOfGenes;
	population = std::map<int, Chromosome>();
	individualProbabilities = std::vector<double>(POPULATION_SIZE, 0);
	previousProbabilities = std::vector<double>(POPULATION_SIZE, 0);
	genesCoordinates = coordinates;
}

void Population::create()
{
	createPopulation();
}

Chromosome Population::getAt(int index)
{
	if (index < POPULATION_SIZE)
	{
		return population[index];
	}
}

void Population::update(int keyIndex, Chromosome value)
{
	if (keyIndex < POPULATION_SIZE)
	{
		population[keyIndex] = value;
	}
}


void Population::print() const
{
	for (auto individualPair : population)
	{
		Chromosome ind = individualPair.second;
		ind.print();
	}
}

double Population::overallFitness() const
{
	double sum = 0;
	for (auto individualPair : population)
	{
		sum += individualPair.second.fitness();
	}
	return sum;
}

void Population::makeSelectionProbabilities()
{
	double overall = overallFitness();
	for (auto individualPair : population)
	{
		Chromosome individual = individualPair.second;
		int index = individualPair.first;
		individualProbabilities[index] = individual.calculateSelectionProbability(overall);
	}
}

void Population::makePreviousProbabilities()
{
	double probability = 0.0;
	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		previousProbabilities[i] = probability;
		probability += individualProbabilities[i];
	}
}

Chromosome Population::fittestIndividual()
{
	std::pair<double, int> fittest(individualProbabilities[0], 0);
	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		if (individualProbabilities[i] > fittest.first)
		{
			fittest.first = individualProbabilities[i];
			fittest.second = i;
		}
	}
	return population[fittest.second];
}

int Population::selectFittestParentIndex(int parentIndex)
{
	std::pair<double, int>  highestProbability(individualProbabilities[0], 0);
	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		if (i != parentIndex && highestProbability.first < individualProbabilities[i])
		{
			highestProbability.first = individualProbabilities[i];
			highestProbability.second = i;
		}
	}
	return highestProbability.second;
}

int Population::selectParentIndex(double randomProbability, int parentIndex)
{
	int index = 0;
	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		if (previousProbabilities[i] >= randomProbability && parentIndex != i)
		{
			return i;
		}
	}
	return index;
}

int Population::selectUnfitIndividual(int otherUnfitIndividual)
{
	std::pair<double, int> lowestProbability(individualProbabilities[0], 0);
	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		if (i != otherUnfitIndividual && lowestProbability.first > individualProbabilities[i])
		{
			lowestProbability.first = individualProbabilities[i];
			lowestProbability.second = i;
		}
	}
	return lowestProbability.second;
}

void Population::createPopulation()
{
	int size;
	while ((size = population.size()) < POPULATION_SIZE)
	{
		Chromosome individual(numberOfGenes, genesCoordinates);
		individual.create();
		population.emplace(size, individual);
	}
}
