#include <iostream>

#include "Algorithm.hpp"

std::vector<Gene> createGenesCoordinates(int numberOfGenes)
{
	std::vector<Gene> genesCoordinates(numberOfGenes);
	std::set<Gene> used;
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());

	for (int i = 0; i < numberOfGenes; i++)
	{
		std::uniform_int_distribution<> rand(1, numberOfGenes);
		Gene gene(rand(generator), rand(generator));
		while (used.count(gene))
		{
			std::uniform_int_distribution<> rand(1, numberOfGenes);
			gene.set(rand(generator), rand(generator));
		}
		genesCoordinates[i] = gene;
		used.insert(gene);
	}
	return genesCoordinates;
}

int main()
{
	int numberOfCities;
	std::cin >> numberOfCities;
	std::vector<Gene> coordinates = createGenesCoordinates(numberOfCities);
	//std::vector<Gene> coordinates = {	Gene(59.011, 98.3061), 
	//									Gene(3.42003, 7.64651), 
	//									Gene(2.3088,25.8841), 
	//									Gene(11.8902, 5.50603), 
	//									Gene(44.3054, 40.0689), 
	//									Gene(80.4767, 32.2054), 
	//									Gene(20.4434, 69.2454), 
	//									Gene(15.7902, 47.2089), 
	//									Gene(98.6976, 42.9433), 
	//									Gene(48.3022, 75.1088)};
	GeneticAlgorithm ga(numberOfCities, coordinates);
	
	//for (int i = 0; i < numberOfCities; i++)
	//{
	//	std::cout << "city " << i + 1 << " " << coordinates[i] << std::endl;
	//}

	ga.reproduction();

	system("pause");
	return 0;
}