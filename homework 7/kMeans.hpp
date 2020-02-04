#pragma once

#include <string>
#include <utility>
#include <vector>

class kMeans
{
public:
	kMeans(std::string, int);

	std::string startClustering();
private:
	int kNumberOfClusters;
	std::vector<std::pair<double, double>> inputData;
	std::vector<int> assignedCentroidPerPoint;
	std::vector<int> updatedAssignedCentroidPerPoint;
	std::vector<std::pair<double, double>> centroids;
	std::vector<std::pair<double, double>> updatedCentroids;

	void loadFromFile(std::string);
	void storeToFile(std::string);
	void cluster();

	std::vector<std::pair<double, double>> initializeCentroids();
	void assignPointsToClosestCentroid();
	void recomputeCentroids();
	bool hasChanged();
	bool hasAlligmentChanged();

	double computeEuclideanDistance(std::pair<double, double>, std::pair<double, double>);
	std::vector<double> computeDistanceFromPointToCentroids(std::pair<double, double>, std::vector<std::pair<double, double>>);

	int findMinimalDistanceID(std::vector<double>);

	std::vector<std::pair<double, double>> findPointsInCluster(int);

	double sumCoordinates(int, std::vector<std::pair<double, double>>);

	std::pair<double, double> findMaxCoordinate();
	void createImage(std::string, int, int);

	void createImageInputData(std::string, int, int);
};