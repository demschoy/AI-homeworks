#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <set>
#include <string>
#include <stack>

using matrix = std::vector<std::vector<int>>;
#define maxNumberOfDirections 4


bool isEmpty(matrix state)
{
	int dimension = int(state.size());
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			if (state[i][j] != 0)
			{
				return false;
			}
		}
	}
	return true;
}


std::pair<int, int> findIndex(matrix puzzle, int number)
{
	int dimension = int(puzzle.size());
	std::pair<int, int> found;

	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			if (puzzle[i][j] == number)
			{
				found.first = i;
				found.second = j;
				return found;
			}
		}
	}
	return found;
}

matrix createGoalState(int dimension, std::pair<int, int> blank)
{
	matrix goal(dimension);
	for (int i = 0; i < dimension; i++)
	{
		goal[i].resize(dimension);
	}
	int numbersCounter = 1;
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			if (blank.first == i && blank.second == j) 
			{
				goal[i][j] = 0;
			}
			else
			{
				goal[i][j] = numbersCounter;
				numbersCounter++;
			}
		}
	}
	
	return goal;
}

int manhattanDistance(matrix start, matrix goal)
{
	int dimension = int(start.size());
	int numberOfBlocks = dimension*dimension - 1;
	std::vector<int> sums(numberOfBlocks + 1);
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			int goalState = goal[i][j];
			std::pair<int, int> difference;
			if (goalState != 0)
			{
				difference = findIndex(start, goalState);
				sums[goalState] = abs(i - difference.first) + abs(j - difference.second);
			}
		}
	}

	int shortestPath = 0;
	for (int i = 1; i < numberOfBlocks + 1; i++)
	{
		shortestPath += sums[i];
	}
	return shortestPath;
}

bool isGoal(matrix node, matrix goal)
{
	int dimension = int(node.size());
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			if (node[i][j] != goal[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

std::pair<int, int> getBlankSpace(matrix state)
{
	int dimension = int(state.size());
	std::pair<int, int> blank(-1, -1);
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			if (state[i][j] == 0) 
			{
				blank.first = i;
				blank.second = j;
				return blank;
			}
		}
	}
	return blank;
}

matrix makeBlockTransfer(matrix state, std::pair<int, int> blank, std::string direction)
{
	int dimension = int(state.size());
	matrix newState(dimension);
	for (int i = 0; i < dimension; i++)
	{
		newState[i].resize(dimension);
	}

	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			if (i == blank.first  && j == blank.second)
			{
				if (direction == "up")
				{
					newState[i][j] = state[i + 1][j];
				}
				if (direction == "down")
				{
					newState[i][j] = state[i - 1][j];
				}
				if (direction == "left")
				{
					newState[i][j] = state[i][j + 1];
				}
				if (direction == "right")
				{
					newState[i][j] = state[i][j - 1];
				}
			}
			else
			{
				newState[i][j] = state[i][j];
			}
		}
	}
	
	if (direction == "up")
	{
		newState[blank.first + 1][blank.second] = 0;
	}
	if (direction == "down")
	{
		newState[blank.first - 1][blank.second] = 0;
	}
	if (direction == "left")
	{
		newState[blank.first][blank.second + 1] = 0;
	}
	if (direction == "right")
	{
		newState[blank.first][blank.second - 1] = 0;
	}
	return newState;
}

std::vector<std::pair<matrix, std::string>> createAdjancentTo(matrix state)
{
	int dimension = int(state.size());
	std::vector<std::pair<matrix, std::string>> adjacency(maxNumberOfDirections);
	
	for (int childInd = 0; childInd < maxNumberOfDirections; childInd++)
	{
		adjacency[childInd].first.resize(dimension);
		for (int row = 0; row < dimension; row++)
		{
			adjacency[childInd].first[row].resize(dimension);
		}
	}
	
	std::pair<int, int> blank = getBlankSpace(state);
	int counter = 0;
	if (blank.first - 1 >= 0)
	{
		matrix downState = makeBlockTransfer(state, blank, "down");
		std::pair<matrix, std::string> down(downState, "down");
		adjacency[counter] = down;
		counter++;
	}
	if (blank.first + 1 < dimension)
	{
		matrix upState = makeBlockTransfer(state, blank, "up");
		std::pair<matrix, std::string> up(upState, "up");
		adjacency[counter] = up;
		counter++;
	}
	if (blank.second - 1 >= 0)
	{
		matrix rightState = makeBlockTransfer(state, blank, "right");
		std::pair<matrix, std::string> right(rightState, "right");
		adjacency[counter] = right;
		counter++;
	}
	if (blank.second + 1 < dimension)
	{
		matrix leftState = makeBlockTransfer(state, blank, "left");
		std::pair<matrix, std::string> left(leftState, "left");
		adjacency[counter] = left;
		counter++;
	}

	return adjacency;
}

std::pair<bool, int> depthLimitedSearch(int bound, int levelBound, int costToTheNode, std::set<matrix>& visited, std::stack<matrix>& path, std::vector<std::string>& directionsPath, matrix goalState)
{
	matrix currentNode = path.top();

	visited.insert(currentNode);
	int totalCost = costToTheNode + manhattanDistance(currentNode, goalState);

	if (isGoal(currentNode, goalState))
	{
		std::pair<bool, int> result(true, totalCost);
		return result;
	}

	if (totalCost > bound || costToTheNode >= levelBound) 
	{
		std::pair<bool, int> result(false, totalCost);
		return result;
	}

	std::vector<std::pair<matrix, std::string>> adjacency = createAdjancentTo(currentNode);
	std::pair<bool, int> result(false, totalCost);
	int minimum = INT_MAX;
	for (std::pair<matrix, std::string> child : adjacency)
	{
		if (isEmpty(child.first) == false && visited.count(child.first) == 0)
		{
			path.push(child.first);
			directionsPath.push_back(child.second);
			int newCost = costToTheNode + manhattanDistance(currentNode, child.first);
			std::pair<bool, int> next = depthLimitedSearch(bound, levelBound, newCost, visited, path, directionsPath, goalState);
			if (next.first)
			{
				return next;
			}
			if (next.second <= minimum)
			{
				minimum = next.second;
			}
			visited.erase(child.first);
			path.pop();
			directionsPath.pop_back();
		}
	}
	result.second = minimum;
	return result;
}

std::pair<int, std::vector<std::string>> idaStar(matrix startState, matrix goalState)
{
	int dimension = int(startState.size());
	int numberOfBlocks = dimension * dimension - 1;
	
	int bound = manhattanDistance(startState, goalState);
	std::vector<std::string> directionsPath;
	std::stack<matrix> path;
	path.push(startState);

	int i = 1, shortestPath = 0;
	while (i)
	{
		std::set<matrix> visited;
		int levelBound = i;
		std::pair<bool, int> result = depthLimitedSearch(bound, levelBound, 0, visited, path, directionsPath, goalState);
		
		if (result.first)
		{
			std::pair<int, std::vector<std::string>> path(result.second, directionsPath);
			return path;
		}
		bound = result.second;
		i++;
	}
		
	std::pair<int, std::vector<std::string>> result(shortestPath, directionsPath);
	return result;
}

int main()
{
	int numberOfBlocks;
	std::cin >> numberOfBlocks;
	std::pair<int, int> blank;
	std::cin >> blank.first;
	int dimension = int(sqrt(numberOfBlocks + 1));
	if (blank.first == -1)
	{
		blank.first = dimension - 1;
		blank.second = dimension - 1;
	}
	else
	{
		std::cin >> blank.second;
	}
	matrix startPuzzle(dimension);

	for (int i = 0; i < dimension; i++)
	{
		startPuzzle[i].resize(dimension);
	}

	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			std::cin >> startPuzzle[i][j];
		}
	}

	matrix goalPuzzle = createGoalState(dimension, blank);
	std::pair<int, std::vector<std::string>> result = idaStar(startPuzzle, goalPuzzle);
	
	std::cout << result.first << std::endl;
	int size = int(result.second.size());
	for (int i = 0; i < size; i++)
	{
		std::cout << result.second[i]<<std::endl;
	}
	system("pause");

	return 0;
}