#include <fstream>
#include <iostream>
#include <algorithm>

#include "NaiveBayesClassifier.hpp"

#define REPUBLICAN "republican"
#define DEMOCRAT "democrat"
#define ATTR_VALUE_POSITIVE "y"
#define ATTR_VALUE_NEGATIVE "n"
#define ATTR_VALUE_NEUTRAL "?"

std::vector<std::vector<int>> loadFromFile(std::string filename)
{
	std::fstream file(filename, std::fstream::in);

	std::vector<std::vector<int>> inputData;
	std::string word;
	std::getline(file, word, ',');
	while (word == REPUBLICAN || word == DEMOCRAT || !file.eof())
	{
		std::vector<int> example;
		if (word == REPUBLICAN)
		{
			example.push_back(ID::republican);
		}
		else
		{
			example.push_back(ID::democrat);
		}

		std::getline(file, word, ',');
		while (word == ATTR_VALUE_POSITIVE || word == ATTR_VALUE_NEGATIVE || word == ATTR_VALUE_NEUTRAL)
		{
			if (word == ATTR_VALUE_POSITIVE)
			{
				example.push_back(ID::positive);
			}
			else if (word == ATTR_VALUE_NEGATIVE)
			{
				example.push_back(ID::negative);
			}
			else if (word == ATTR_VALUE_NEUTRAL)
			{
				example.push_back(ID::neutral);
			}
			if (!file.eof())
			{
				std::getline(file, word, ',');
			}

			if (word == "y\nrepublican" || word == "n\nrepublican" || word == "y\ndemocrat" || word == "n\ndemocrat" || word == "?\nrepublican" || word == "?\ndemocrat")
			{
				if (word == "y\nrepublican" || word == "y\ndemocrat")
				{
					example.push_back(ID::positive);
				}
				else if (word == "n\nrepublican" || "n\ndemocrat")
				{
					example.push_back(ID::negative);
				}
				else if (word == "?\nrepublican" || "?\ndemocrat")
				{
					example.push_back(ID::neutral);
				}

				if (word == "n\nrepublican" || word == "y\nrepublican" || word == "?\nrepublican")
				{
					inputData.push_back(example);
					word = REPUBLICAN;
				}
				else if (word == "y\ndemocrat" || word == "n\ndemocrat" || word == "?\ndemocrat")
				{
					inputData.push_back(example);
					word = DEMOCRAT;
				}
			}
			if (word == "y\n" || word == "n\n" || word == "?\n")
			{
				if (word == "y\n")
				{
					example.push_back(ID::positive);
				}
				else if (word == "n\n")
				{
					example.push_back(ID::negative);
				}
				else if (word == "?\n")
				{
					example.push_back(ID::neutral);
				}

				inputData.push_back(example);
			}
		}
	}
	file.close();
	return inputData;
}

void splitFolds(int cycleFold, std::vector<std::vector<int>> data, std::vector<std::vector<int>>& trainingSet, std::vector<std::vector<int>>& testSet)
{
	int size = data.size();
	int fold = size / 10;
	if (cycleFold < 9)
	{
		for (int i = 0; i < fold * cycleFold; i++)
		{
			trainingSet.push_back(data[i]);
		}
		for (int i = cycleFold * fold; i < (cycleFold + 1) * fold; i++)
		{
			testSet.push_back(data[i]);
		}
		for (int i = (cycleFold + 1) * fold; i < size; i++)
		{
			trainingSet.push_back(data[i]);
		}
	}
	else if (cycleFold == 9)
	{
		for (int i = 0; i < fold * 9; i++)
		{
			trainingSet.push_back(data[i]);
		}
		for (int i = 9 * fold; i < size; i++)
		{
			testSet.push_back(data[i]);
		}
	}
}

void tenFoldCrossValidation(std::vector<std::vector<int>> data)
{
	std::random_shuffle(data.begin(), data.end());
	double mean = 0.0;
	for (int i = 0; i < 10; i++)
	{
		std::vector<std::vector<int>> trainingSet, testSet;
		splitFolds(i, data, trainingSet, testSet);
		NaiveBayesClassifier alg(trainingSet);
		double accuracy = alg.test(testSet);
		std::cout << "Fold iteration " << i << ", accuracy = " << accuracy<<std::endl;
		mean += accuracy;
	}
	mean /= 10;
	std::cout << "\nMean accuracy = " << mean << std::endl;
}

int main()
{
	std::vector<std::vector<int>> inputData = loadFromFile("house-votes-84.data");
	tenFoldCrossValidation(inputData);

	system("pause");

	return 0;
}