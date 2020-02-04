#pragma once

#include <string>
#include <unordered_map>
#include <vector>

enum ID
{
	republican = 10,
	democrat = 20,
	positive = 1,
	negative = 0,
	neutral = -1,
	positiveID = 100,
	negativeID = 200,
	neutralID = 300
};

class NaiveBayesClassifier
{
public:
	NaiveBayesClassifier(std::vector<std::vector<int>>);
	int predict(std::vector<int>);

	double test(std::vector<std::vector<int>>);
private:
	std::unordered_map<int, double> classesProbabilities; // map< classID, P(C = classID) >
	std::unordered_map<int, std::unordered_map<int, double>> attributesPerClass; 
														// map< classID, map < attrID, P(X = attriID | C = classID) >>

	double calculateClassProbability(int, std::vector<std::vector<int>>);
	double calculateAttributePerClassProbability(int, int, int, std::vector<std::vector<int>>);
};