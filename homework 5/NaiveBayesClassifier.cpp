#include "NaiveBayesClassifier.hpp"

#include <fstream>
#include <iostream>
#include <cmath>

NaiveBayesClassifier::NaiveBayesClassifier(std::vector<std::vector<int>> inputData)
{
	double republicanProbability = calculateClassProbability(ID::republican, inputData);
	classesProbabilities.emplace(ID::republican, republicanProbability);
	double democratProbability = calculateClassProbability(ID::democrat, inputData);
	classesProbabilities.emplace(ID::democrat, democratProbability);

	std::unordered_map<int, double> republicanAttributesAndProbabilities, democratAttributesAndProbabilities;
	int numberOfAttributes = inputData[0].size();
	int size = inputData.size();
	for (int entries = 0; entries < size; entries++)
	{
		for (int i = 1; i < numberOfAttributes; i++)
		{
			int classID = inputData[entries][0];
			
			double probabilityP = calculateAttributePerClassProbability(classID, i, ID::positive, inputData);
			double probabilityN = calculateAttributePerClassProbability(classID, i, ID::negative, inputData);
			double probabilityNeutral = calculateAttributePerClassProbability(classID, i, ID::neutral, inputData);

			if (classID == ID::republican) 
			{
				republicanAttributesAndProbabilities[i + ID::positiveID] = probabilityP;
				republicanAttributesAndProbabilities[i + ID::negativeID] = probabilityN;
				republicanAttributesAndProbabilities[i + ID::neutralID] = probabilityNeutral;
			} 
			else if (classID == ID::democrat)
			{
				democratAttributesAndProbabilities[i + ID::positiveID] = probabilityP;
				democratAttributesAndProbabilities[i + ID::negativeID] = probabilityN;
				democratAttributesAndProbabilities[i + ID::neutralID] = probabilityNeutral;
			}
		}
	}

	attributesPerClass.emplace(ID::republican, republicanAttributesAndProbabilities);
	attributesPerClass.emplace(ID::democrat, democratAttributesAndProbabilities);
}

int NaiveBayesClassifier::predict(std::vector<int> instance)
{
	int numberOfAttributes = instance.size();
	
	double logaritmicProbabilityForClassRepublican = log(classesProbabilities[ID::republican]),
		logaritmicProbabilityForClassDemocrat = log(classesProbabilities[ID::democrat]);

	for (int i = 1; i < numberOfAttributes; i++)
	{
		int feature = instance[i];
		int attributeID = i;
		if (feature == ID::positive)
		{
			attributeID += positiveID;
		}
		else if (feature == ID::negative)
		{
			attributeID += negativeID;
		}
		else if (feature == ID::neutral)
		{
			attributeID += neutralID;
		}

		std::unordered_map<int, double> attrRepublican = attributesPerClass[ID::republican];
		logaritmicProbabilityForClassRepublican += log(attrRepublican[attributeID]);

		std::unordered_map<int, double> attrDemocrat = attributesPerClass[ID::democrat];
		logaritmicProbabilityForClassDemocrat += log(attrDemocrat[attributeID]);
	}

	if (logaritmicProbabilityForClassDemocrat > logaritmicProbabilityForClassRepublican)
	{
		return ID::democrat;
	}
	else if (logaritmicProbabilityForClassDemocrat < logaritmicProbabilityForClassRepublican)
	{
		return ID::republican;
	}
}

double NaiveBayesClassifier::test(std::vector<std::vector<int>> testSet)
{
	int size = testSet.size();
	int correct = 0;
	for (int entry = 0; entry < size; entry++)
	{
		if (testSet[entry][0] == predict(testSet[entry]))
		{
			correct++;
		}
	}
	double accuracy = double(correct) / size;
	return accuracy;
}

double NaiveBayesClassifier::calculateClassProbability(int id, std::vector<std::vector<int>> inputData)
{
	int size = inputData.size();
	int count = 1;
	for (int i = 0; i < size; i++)
	{
		if (inputData[i][0] == id)
		{
			count++;
		}
	}
	size++;
	double probability = double(count)/size;
	return probability;
}

double NaiveBayesClassifier::calculateAttributePerClassProbability(int classID, int attributeID, int attributeValue, std::vector<std::vector<int>> inputData)
{
	int count = 1, size = inputData.size(), countClasses = 1;
	for (int entry = 0; entry < size; entry++)
	{
		if(inputData[entry][0] == classID && inputData[entry][attributeID] == attributeValue)
		{
			count++;
		}
		if (inputData[entry][0] == classID)
		{
			countClasses++;
		}
	}
	double probability = double(count) / countClasses;
	return probability;
}
