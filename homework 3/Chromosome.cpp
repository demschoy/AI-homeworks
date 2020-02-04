#include "Chromosome.hpp"

Chromosome::Chromosome()
{
	numberOfGenes = 1;
	individual = std::vector<Gene>(numberOfGenes);
}

Chromosome::Chromosome(int _numberOfGenes, std::vector<Gene> _coordinates)
{
	numberOfGenes = _numberOfGenes;
	individual = std::vector<Gene>(numberOfGenes);
	coordinates = _coordinates;
}

Chromosome::Chromosome(std::vector<Gene> _individual)
{
	individual = _individual;
	numberOfGenes = individual.size();
}

void Chromosome::create()
{
	createIndividual(coordinates);
}

int Chromosome::getNumberOfGenes() const
{
	return numberOfGenes;
}

std::vector<Gene> Chromosome::getIndividual() const
{
	return individual;
}

void Chromosome::mutate(std::vector<Gene> mutatedIndividual)
{
	if (mutatedIndividual.size() == individual.size())
	{
		individual = mutatedIndividual;
	}
}

double Chromosome::distance() const
{
	double distance = 0;

	for (int i = 1; i < numberOfGenes; i++)
	{
		int a = individual[i - 1].getX() - individual[i].getX();
		int b = individual[i - 1].getY() - individual[i].getY();
		distance += sqrt(a*a + b * b);
	}
	return distance;
}

double Chromosome::fitness() const
{
	double pathDistance = distance();
	double fitnessCoef = 1 / pathDistance;
	return fitnessCoef;
}

double Chromosome::calculateSelectionProbability(double overallFitness) const
{
	double individualFitness = fitness();
	double probability = individualFitness / overallFitness;
	return probability;
}

void Chromosome::print() const
{
	for (int i = 0; i < numberOfGenes; i++)
	{
		std::cout << individual[i] << " ";
	}
	std::cout << std::endl;
}


void Chromosome::createIndividual(std::vector<Gene> coordinates)
{
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());

	std::set<int> usedIndexes;

	for (int i = 0; i < numberOfGenes; i++)
	{
		std::uniform_int_distribution<> rand(0, numberOfGenes - 1);
		int index = rand(generator);
		while (usedIndexes.count(index))
		{
			std::uniform_int_distribution<> rand(0, numberOfGenes - 1);
			index = rand(generator);
		}
		individual[i] = coordinates[index];
		usedIndexes.insert(index);
	}
}


bool operator<(const Chromosome& left, const Chromosome& right)
{
	return left.individual < right.individual;
}
