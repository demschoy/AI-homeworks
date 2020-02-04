#include "kMeans.hpp"

#include "bitmap/bitmap/bitmap_image.hpp"

#include <fstream>
#include <iostream>
#include <cmath>
#include <random>
#include <set>
#include <map>

kMeans::kMeans(std::string filename, int _k) : kNumberOfClusters(_k)
{
	loadFromFile(filename);
	int datasetSize = inputData.size();
	assignedCentroidPerPoint = std::vector<int>(datasetSize, -1);
	updatedAssignedCentroidPerPoint = std::vector<int>(datasetSize);
	centroids = std::vector<std::pair<double, double>>(kNumberOfClusters);
	updatedCentroids = std::vector<std::pair<double, double>>(kNumberOfClusters);
}

std::string kMeans::startClustering()
{
	cluster();
	std::string name = "output.txt";
	storeToFile(name);
	name = "output.bmp";
	std::string inputName = "input.bmp";
	std::pair<double, double> maxCoordinate = findMaxCoordinate();
	int width, height;
	if (maxCoordinate.first == (int)maxCoordinate.first)
	{
		width = maxCoordinate.first / 1000 + 10;
		height = maxCoordinate.second / 1000 + 10;
	}
	else
	{
		width = (maxCoordinate.first + 0.1) * 100;
		height = (maxCoordinate.second + 0.1) * 100;
	}
	createImageInputData(inputName, width, height);
	createImage(name, width, height);
	
	return name;
}

void kMeans::cluster()
{
	centroids = initializeCentroids();

	assignPointsToClosestCentroid();
	assignedCentroidPerPoint = updatedAssignedCentroidPerPoint;
	recomputeCentroids();
	bool emptyCluster = false;
	for (int i = 0; i < kNumberOfClusters; i++)
	{
		std::vector<std::pair<double, double>> points = findPointsInCluster(i);
		if (points.size() == 0)
		{
			emptyCluster = true;
		}
	}
	
	while(hasChanged() || hasAlligmentChanged() || emptyCluster)
	{
		emptyCluster = false;
		centroids = updatedCentroids;
		assignedCentroidPerPoint = updatedAssignedCentroidPerPoint;
		assignPointsToClosestCentroid();
		recomputeCentroids();
		
		for (int i = 0; i < kNumberOfClusters; i++)
		{
			std::vector<std::pair<double, double>> points = findPointsInCluster(i);
			if (points.size() == 0)
			{
				emptyCluster = true;
			}
		}

		if (!hasChanged() && !hasAlligmentChanged() && emptyCluster)
		{
			centroids = initializeCentroids();
			assignPointsToClosestCentroid();
			recomputeCentroids();
		}
	}
}

std::vector<std::pair<double, double>> kMeans::initializeCentroids()
{
	std::random_device rand;
	std::mt19937 generator(rand());
	std::uniform_int_distribution<int> coord(0, inputData.size() - 1);

	std::set<int> usedIndex;
	std::vector<std::pair<double, double>> centroids(kNumberOfClusters);
	for (int i = 0; i < kNumberOfClusters; i++)
	{
		int randomIndex = coord(generator);
		while (usedIndex.count(randomIndex) != 0)
		{
			randomIndex = coord(generator);
		}
		usedIndex.insert(randomIndex);
		centroids[i].first = inputData[randomIndex].first;
		centroids[i].second = inputData[randomIndex].second;
	}
	return centroids;
}

void kMeans::assignPointsToClosestCentroid()
{
	int datasetSize = inputData.size();

	for (int pointID = 0; pointID < datasetSize; pointID++)
	{
		std::vector<double> distances = computeDistanceFromPointToCentroids(inputData[pointID], centroids);
		int closestCentroidID = findMinimalDistanceID(distances);
		updatedAssignedCentroidPerPoint[pointID] = closestCentroidID;
	}
}

void kMeans::recomputeCentroids()
{
	for (int cluster = 0; cluster < kNumberOfClusters; cluster++)
	{
		std::vector<std::pair<double, double>> points = findPointsInCluster(cluster);

		int first = 1, second = 2;
		int numberOfPoints = points.size();
		if (numberOfPoints == 0)
		{
			numberOfPoints = 1;
		}
		double firstCoordinatesSum = sumCoordinates(first, points), secondCoordinatesSum = sumCoordinates(second, points);
		updatedCentroids[cluster].first = firstCoordinatesSum / numberOfPoints;
		updatedCentroids[cluster].second = secondCoordinatesSum / numberOfPoints;
	}
}

bool kMeans::hasChanged()
{
	for (int cluster = 0; cluster < kNumberOfClusters; cluster++)
	{
		if (centroids[cluster].first != updatedCentroids[cluster].first || centroids[cluster].second != updatedCentroids[cluster].second)
		{
			return true;
		}
	}

	return false;
}

bool kMeans::hasAlligmentChanged()
{
	int size = inputData.size();
	for (int i = 0; i < size; i++)
	{
		if (assignedCentroidPerPoint[i] != updatedAssignedCentroidPerPoint[i])
		{
			return true;
		}
	}
	return false;
}

double kMeans::computeEuclideanDistance(std::pair<double, double> firstPoint, std::pair<double, double> secondPoint)
{
	double differenceFirstCoordinate = firstPoint.first - secondPoint.first,
		differenceSecondCoordinate = firstPoint.second - secondPoint.second,
		squaredSum = differenceFirstCoordinate * differenceFirstCoordinate + differenceSecondCoordinate * differenceSecondCoordinate,
		distance = sqrt(squaredSum);
	return distance;
}

std::vector<double> kMeans::computeDistanceFromPointToCentroids(std::pair<double, double> point, std::vector<std::pair<double, double>> centroids)
{
	std::vector<double> distances(kNumberOfClusters);
	for (int i = 0; i < kNumberOfClusters; i++)
	{
		distances[i] = computeEuclideanDistance(point, centroids[i]);
	}
	return distances;
}

int kMeans::findMinimalDistanceID(std::vector<double> data)
{ 
	std::pair<double, int> minimal(INT_MAX, -1);
	int size = data.size();
	for (int i = 0; i < size; i++)
	{
		if (data[i] < minimal.first)
		{
			minimal.first = data[i];
			minimal.second = i;
		}
	}
	return minimal.second;
}

std::vector<std::pair<double, double>> kMeans::findPointsInCluster(int clusterID)
{
	std::vector<std::pair<double, double>> assignedPoints;
	int datasetSize = inputData.size();
	for (int point = 0; point < datasetSize; point++)
	{
		if (assignedCentroidPerPoint[point] == clusterID)
		{
			assignedPoints.push_back(inputData[point]);
		}
	}
	
	return assignedPoints;
}

double kMeans::sumCoordinates(int coordinationIndex, std::vector<std::pair<double, double>> points)
{
	double sum = 0;
	int size = points.size();
	for (int i = 0; i < size; i++)
	{
		if (coordinationIndex == 1)
		{
			sum += points[i].first;
		}
		else if (coordinationIndex == 2)
		{
			sum += points[i].second;
		}
	}
	return sum;
}

std::pair<double,double> kMeans::findMaxCoordinate()
{
	std::pair<double, double> max(INT32_MIN, INT32_MIN);
	for (std::pair<double, double> point : inputData)
	{
		if (max.first < point.first)
		{
			max.first = point.first;
		}
		if (max.second < point.second)
		{
			max.second = point.second;
		}
	}
	return max;
}

void kMeans::createImage(std::string name, int width, int height)
{
	bitmap_image image(width, height);
	image.clear();
	image.set_all_channels(255, 255, 255);

	image_drawer draw(image);
	draw.pen_width(1);

	std::vector<std::vector<int>> colors = { std::vector<int> {0, 0, 128}, //blue 0
											std::vector<int> {204, 0, 0}, //red 1
											std::vector<int> {0, 204, 0}, //green 2
 											std::vector<int> {255, 51, 153}, //pink 3
											std::vector<int> {204, 102, 0}, //orange 4 
											std::vector<int> {76, 0, 153}, //purple 5 
											std::vector<int> {51, 255, 255}, //cyan 6
											std::vector<int> {32, 32, 32} }; //gray 7
	std::map<int, int> clusterInformationColor;
	std::set<int> usedColors;

	for (int cluster = 0; cluster < kNumberOfClusters; cluster++)
	{
		std::random_device rand;
		std::mt19937 generator(rand());
		std::uniform_int_distribution<int> indices(0, colors.size() - 1);

		int index = indices(generator);
		while (usedColors.count(index) != 0)
		{
			index = indices(generator);
		}
		usedColors.insert(index);
		clusterInformationColor.emplace(cluster, index);
	}

	int size = inputData.size();
	for (int i = 0; i < size; i++)
	{
		std::pair<double, double> point = inputData[i];
		int x, y;
		if (point.first == (int)point.first)
		{
			x = point.first / 1000;
			y = point.second / 1000;
		}
		else
		{
			x = point.first * 100;
			y = point.second * 100;
		}
		int cluster = assignedCentroidPerPoint[i];
		int colorIndex = clusterInformationColor[cluster];
		std::vector<int> color = colors[colorIndex];
		draw.pen_color(color[0], color[1], color[2]);
		draw.circle(x, y, 3);
		
	}
	image.save_image(name);
}

void kMeans::createImageInputData(std::string name, int width, int height)
{
	bitmap_image image(width, height);
	image.set_all_channels(255, 255, 255);
	image_drawer draw(image);
	draw.pen_width(1);

	int size = inputData.size();
	for (int i = 0; i < size; i++)
	{
		std::pair<double, double> point = inputData[i];
		int x, y;
		if (point.first == (int)point.first)
		{
			x = point.first / 1000;
			y = point.second / 1000;
		}
		else
		{
			x = point.first * 100;
			y = point.second * 100;
		}
		draw.pen_color(0, 0, 0);
		draw.circle(x, y, 3);
	}
	image.save_image(name);
}

void kMeans::loadFromFile(std::string filename)
{
	double x, y; 
	std::fstream file(filename, std::fstream::in);
	while (!file.eof())
	{
		file >> x;
		file >> y; 
		std::pair<double, double> coordinates(x, y);
		inputData.push_back(coordinates);
	}
}

void kMeans::storeToFile(std::string filename)
{
	std::fstream file(filename, std::fstream::out);
	int size = assignedCentroidPerPoint.size();
	for (int i = 0; i < size; i++)
	{
		file << inputData[i].first << " " << inputData[i].second << " " << assignedCentroidPerPoint[i] << std::endl;
	}
	file.close();
}
