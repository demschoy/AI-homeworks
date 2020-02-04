#include "Algorithm.hpp"

GeneticAlgorithm::GeneticAlgorithm(int _numberOfGenes, std::vector<Gene> _coordinates)
{
	numberOfGenes = _numberOfGenes;
	std::vector<Gene> coordinates = _coordinates;
	for (int i = 0; i < numberOfGenes; i++)
	{
		std::cout << "city " << i + 1 << " " << coordinates[i] << std::endl;
	}
	population = Population(numberOfGenes, coordinates);
	population.create();
}

Population GeneticAlgorithm::getPopulation() const
{
	return population;
}

void GeneticAlgorithm::reproduction()
{
	int iteration = 0;
	int noImprovementCounter = 0;
	while (noImprovementCounter < MAX_NO_IMPROVEMENT_ITERATIONS)
	{
		double currentlyBestFitness = 0;
		if (iteration % 10 == 0  || iteration == 2 || iteration == 3 || iteration == 5)
		{
			Chromosome currentlyBest = population.fittestIndividual();
			std::cout << "Iteration #" << iteration << ", currently best path " << currentlyBest.distance() << "\n\n";
		}

		int breedingPercantage = POPULATION_SIZE * 0.40;
		int numberOfPairs = 0;
		double overallFitnessPopulation = population.overallFitness();
		double currentlyBestPath = population.fittestIndividual().distance();

		while (numberOfPairs < (breedingPercantage / 2))
		{
			std::pair<Chromosome, Chromosome> parents = selection();
			std::pair<Chromosome, Chromosome> children = crossover(parents);

			numberOfPairs++;

			if (children.first.fitness() < parents.first.fitness() || children.first.fitness() < parents.second.fitness())
			{
				mutation(children.first);
			}
			if (children.second.fitness() < parents.first.fitness() || children.second.fitness() < parents.second.fitness())
			{
				mutation(children.second);
			}

			updatePopulation(children);
		}

		double mutatedPopulationOverallFitnes = population.overallFitness();
		
		if (overallFitnessPopulation >= mutatedPopulationOverallFitnes)
		{
			noImprovementCounter++;
		}
		else
		{
			noImprovementCounter = 0;
		}

		iteration++;
	}

	Chromosome best = population.fittestIndividual();
	std::cout << "Last iteration #" << iteration << ", currently best path " << best.distance() << "\nPath: ";
	best.print();
	std::cout << std::endl;
}

std::pair<Chromosome, Chromosome> GeneticAlgorithm::selection()
{
	population.makeSelectionProbabilities();
	population.makePreviousProbabilities();

	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<> rand(0, 0.99);
	double randomProbability = rand(generator);

	int firstParentIndex = population.selectParentIndex(randomProbability);
//	int firstParentIndex = population.selectFittestParentIndex();
	Chromosome firstParent = population.getAt(firstParentIndex);

	randomProbability = rand(generator);

	int secondParentIndex = population.selectParentIndex(randomProbability, firstParentIndex);
//	int secondParentIndex = population.selectFittestParentIndex(firstParentIndex);
	Chromosome secondParent = population.getAt(secondParentIndex);

	std::pair<Chromosome, Chromosome> parents(firstParent, secondParent);
	return parents;
}

std::pair<Chromosome, Chromosome> GeneticAlgorithm::crossover(std::pair<Chromosome, Chromosome> parents)
{
	//two-point crossover
	Chromosome firstParent = parents.first, secondParent = parents.second;

	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_int_distribution<> rand(0, numberOfGenes - 2);
	
	int crossoverPointIndex = rand(generator);
	int crossoverSecondPointIndex = rand(generator);
	while (crossoverPointIndex != crossoverSecondPointIndex)
	{
		crossoverSecondPointIndex = rand(generator);
	}
	if (crossoverPointIndex > crossoverSecondPointIndex)
	{
		int tmp = crossoverPointIndex;
		crossoverPointIndex = crossoverSecondPointIndex;
		crossoverSecondPointIndex = tmp;
	}

//	std::cout << "\ncrossover index = " << crossoverPointIndex << std::endl;

	std::vector<Gene> firstChild(numberOfGenes), secondChild(numberOfGenes);
	std::vector<Gene> firstParentIndividual = firstParent.getIndividual(), secondParentIndividual = secondParent.getIndividual();
	std::set<Gene> firstChildTaken, secondChildTaken;
	for (int i = crossoverPointIndex; i <= crossoverSecondPointIndex; i++)
	{
		firstChild[i] = firstParentIndividual[i];
		firstChildTaken.insert(firstChild[i]);

		secondChild[i] = secondParentIndividual[i];
		secondChildTaken.insert(secondChild[i]);
	}

	crossoverSwitchingParents(firstChild, secondParentIndividual, crossoverSecondPointIndex, firstChildTaken);
	crossoverSwitchingParents(secondChild, firstParentIndividual, crossoverSecondPointIndex, secondChildTaken);

	Chromosome firstKid(firstChild), secondKid(secondChild);
	std::pair<Chromosome, Chromosome> childs(firstKid, secondKid);
	return childs;
}

void GeneticAlgorithm::mutation(Chromosome& individual)
{
	//swap
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_int_distribution<> rand(0, numberOfGenes - 1);

	int firstIndexToBeSwapped = rand(generator);
	int secondIndexToBeSwapped = rand(generator);
	while (firstIndexToBeSwapped == secondIndexToBeSwapped)
	{
		secondIndexToBeSwapped = rand(generator);
	}

	std::vector<Gene> mutated = individual.getIndividual();
	Gene tmp = mutated[firstIndexToBeSwapped];
	mutated[firstIndexToBeSwapped] = mutated[secondIndexToBeSwapped];
	mutated[secondIndexToBeSwapped] = tmp;

	individual.mutate(mutated);
}

void GeneticAlgorithm::updatePopulation(std::pair<Chromosome, Chromosome> children)
{
	int unfitIndividual = population.selectUnfitIndividual();
	int secondUnfitIndividual = population.selectUnfitIndividual(unfitIndividual);

	population.update(unfitIndividual, children.first);
	population.update(secondUnfitIndividual, children.second);
}

void GeneticAlgorithm::crossoverSwitchingParents(std::vector<Gene>& child, std::vector<Gene> parent, int crossoverPointIndex, std::set<Gene> childTaken)
{
	int childIndex = crossoverPointIndex + 1;
	int i = crossoverPointIndex + 1;
	while (childTaken.size() != numberOfGenes)
	{
		Gene newGene = parent[i];
		if (childTaken.count(newGene) == 0)
		{
			child[childIndex] = newGene;
			childTaken.insert(newGene);
			childIndex++;
			i++;
			if (i == numberOfGenes)
			{
				i = 0;
			}
			if (childIndex == numberOfGenes)
			{
				childIndex = 0;
			}
		}
		else
		{
			while (childTaken.count(parent[i]) != 0)
			{
				i++;
				if (i == numberOfGenes)
				{
					i = 0;
				}
			}
			child[childIndex] = parent[i];
			childTaken.insert(child[childIndex]);
			childIndex++;
			if (childIndex == numberOfGenes)
			{
				childIndex = 0;
			}

		}
	}
}
