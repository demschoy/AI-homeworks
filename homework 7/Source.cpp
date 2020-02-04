#include <iostream>

#include "kMeans.hpp"

int main()
{
	int kNormal = 4, kUnbalanced = 8;
	kMeans algorithm("../unbalance/unbalance.txt", kUnbalanced);
//	kMeans algorithm("../normal/normal.txt", kNormal);
	std::string file = algorithm.startClustering();
	system("pause");
	return 0;
}
