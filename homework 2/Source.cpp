#include <iostream>
#include <vector>
#include <set>
#include <stdlib.h> 
#include <ctime>
#include <chrono>
#include <random>

std::random_device randomDevice;
std::mt19937 generator(randomDevice());

void printQueens(std::vector<int> queens)
{
	int numberOfQueens = queens.size();
	std::vector<std::vector<int>> q(numberOfQueens);

	for (int i = 0; i < numberOfQueens; i++)
	{
		q[i].resize(numberOfQueens);
	}

	for (int col = 0; col < numberOfQueens; col++)
	{
		for (int row = 0; row < numberOfQueens; row++)
		{
			if (col == queens[row])
			{
				q[row][col] = 1;
			}
			else
			{
				q[row][col] = 0;
			}
		}
	}

	for (int col = 0; col < numberOfQueens; col++)
	{
		for (int row = 0; row < numberOfQueens; row++)
		{
			if (q[row][col])
			{
				std::cout << "* ";
			}
			else
			{
				std::cout << "_ ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

class Queens
{
public:
	Queens(int numberOfQueens)
	{
		this->numberOfQueens = numberOfQueens;
		queensPerColumn = randomInit();
	}

	std::vector<int> minConflicts(int maxIterations)
	{
		calculateQueens(queensPerRow, primaryQueens, secondaryQueens);

		int i = 1;
		while (i <= maxIterations)
		{
			i++;
			std::pair<int, int> maxConflictsColumn = getColumnWithMaxConflicts();
			int column = maxConflictsColumn.first;
			int conflicts = maxConflictsColumn.second;

			if (conflicts == 0)
			{
				return queensPerColumn;
			}

			int maxConflictsRow = queensPerColumn[column];
			int row = getRowWithMinConflicts(column);
			queensPerColumn[column] = row;

			if (!hasConflicts())
			{
				return queensPerColumn;
			}

			if (row == maxConflictsRow)
			{
				continue;
			}

			int maxConflictsPrimaryIndex = codePrimaryDiagonal(column - maxConflictsRow);
			int conflictsPrimaryIndex = codePrimaryDiagonal(column - row);
			primaryQueens[conflictsPrimaryIndex]++;
			primaryQueens[maxConflictsPrimaryIndex]--;

			int maxConflictsSecondaryIndex = column + maxConflictsRow;
			int conflictsSecondaryIndex = column + row;
			secondaryQueens[conflictsSecondaryIndex]++;
			secondaryQueens[maxConflictsSecondaryIndex]--;

			queensPerRow[row]++;
			queensPerRow[maxConflictsRow]--;
		}

		if (hasConflicts())
		{
			return restart(maxIterations);
		}
	}

private:
	int numberOfQueens;
	std::vector<int> queensPerColumn;
	std::vector<int> queensPerRow;
	std::vector<int> primaryQueens;
	std::vector<int> secondaryQueens;

	std::vector<int> randomInit()
	{
		queensPerColumn = std::vector<int>(numberOfQueens);
		std::set<int> usedRows;
		for (int i = 0; i < numberOfQueens; i++)
		{
			std::uniform_int_distribution<> rand(0, numberOfQueens - 1);
			int row = rand(generator);
			while (usedRows.count(row))
			{
				std::uniform_int_distribution<> rand(0, numberOfQueens - 1);
				row = rand(generator);
			}
			queensPerColumn[i] = row;
			usedRows.insert(row);
		}
		return queensPerColumn;
	}

	std::vector<int> restart(int maxIterations)
	{
		if (maxIterations + numberOfQueens <= 3 * numberOfQueens)
		{
			maxIterations += numberOfQueens;
		}
		queensPerColumn = randomInit();
		return minConflicts(maxIterations);
	}

	bool hasConflicts()
	{
		std::set<int> rows, diagonals;
		rows.insert(queensPerColumn[0]);
		diagonals.insert(0 + queensPerColumn[0]);
		diagonals.insert(0 - queensPerColumn[0]);

		for (int i = 1; i < numberOfQueens; i++)
		{
			int row = queensPerColumn[i];
			if (rows.count(row) || diagonals.count(i + row) || diagonals.count(i - row))
			{
				return true;
			}
			rows.insert(row);
			diagonals.insert(i + row);
			diagonals.insert(i - row);
		}
		return false;
	}

	int codePrimaryDiagonal(int difference)
	{
		int index;
		if (difference <= 0)
		{
			index = (difference * 2) + (2 * numberOfQueens - 1) - 1;
		}
		if (difference > 0)
		{
			index = (difference * (-2)) + (2 * numberOfQueens - 1);
		}
		return index;
	}

	void calculateQueens(std::vector<int>& queensPerRow, std::vector<int>& primaryQueens, std::vector<int>& secondaryQueens)
	{
		queensPerRow = std::vector<int>(numberOfQueens, 0);
		primaryQueens = std::vector<int>(2 * numberOfQueens - 1, 0);
		secondaryQueens = std::vector<int>(2 * numberOfQueens - 1, 0);

		for (int i = 0; i < numberOfQueens; i++)
		{
			int row = queensPerColumn[i];
			queensPerRow[row]++;

			int primaryIndex = codePrimaryDiagonal(i - queensPerColumn[i]);
			primaryQueens[primaryIndex]++;

			int secondaryIndex = i + queensPerColumn[i];
			secondaryQueens[secondaryIndex]++;
		}
	}

	std::pair<int, int> getColumnWithMaxConflicts()
	{
		std::pair<int, int> maximumConflicts(0, 0);
		std::vector<std::pair<int, int>> conflictCandidates;

		for (int i = 0; i < numberOfQueens; i++)
		{
			int row = queensPerColumn[i];
			int primaryIndex = codePrimaryDiagonal(i - row);
			int secondaryIndex = i + row;

			int conflicts = queensPerRow[row] + primaryQueens[primaryIndex] + secondaryQueens[secondaryIndex] - 3;

			if (conflicts > maximumConflicts.second)
			{
				maximumConflicts.first = i;
				maximumConflicts.second = conflicts;
			}
			if (conflicts == maximumConflicts.second)
			{
				if (conflictCandidates.empty())
				{
					conflictCandidates.push_back(maximumConflicts);
				}
				std::pair<int, int> candidate(i, conflicts);
				conflictCandidates.push_back(candidate);
			}
		}

		if (!conflictCandidates.empty() && conflictCandidates[0].second == maximumConflicts.second)
		{
			std::uniform_int_distribution<> rand(0, conflictCandidates.size() - 1);
			int index = rand(generator);
			return conflictCandidates[index];
		}
		return maximumConflicts;
	}

	int getRowWithMinConflicts(int forColumn)
	{
		int queenRowForColumn = queensPerColumn[forColumn];
		int primaryIndexForColumn = codePrimaryDiagonal(forColumn - queenRowForColumn);
		int secondaryIndexForColumn = forColumn + queenRowForColumn;

		std::pair<int, int> minimalConflicts(queenRowForColumn,
			queensPerRow[queenRowForColumn] + primaryQueens[primaryIndexForColumn] + secondaryQueens[secondaryIndexForColumn] - 3);

		std::vector<std::pair<int, int>> conflictCandidates;
		for (int currentRow = 0; currentRow < numberOfQueens; currentRow++)
		{
			if (currentRow == queenRowForColumn)
			{
				continue;
			}

			int currentPrimaryIndex = codePrimaryDiagonal(forColumn - currentRow);
			int currentSecondaryIndex = forColumn + currentRow;

			int conflict = queensPerRow[currentRow] + primaryQueens[currentPrimaryIndex] + secondaryQueens[currentSecondaryIndex];
			if (conflict < minimalConflicts.second)
			{
				minimalConflicts.first = currentRow;
				minimalConflicts.second = conflict;
			}
			if (conflict == minimalConflicts.second)
			{
				if (conflictCandidates.empty())
				{
					conflictCandidates.push_back(minimalConflicts);
				}
				std::pair<int, int> candidate(currentRow, conflict);
				conflictCandidates.push_back(candidate);
			}
		}

		if (!conflictCandidates.empty() && conflictCandidates[0].second == minimalConflicts.second)
		{
			std::uniform_int_distribution<> rand(0, conflictCandidates.size() - 1);
			int index = rand(generator);
			return conflictCandidates[index].first;
		}
		return minimalConflicts.first;
	}
};

int main()
{
	int numberOfQueens;
	std::cin >> numberOfQueens;

	Queens queens(numberOfQueens);
	auto start = std::chrono::system_clock::now();
	std::vector<int> result = queens.minConflicts(2 * numberOfQueens);
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsedTime = end - start;
	std::cout << "elapse time: " << elapsedTime.count() << "s\n";
//	printQueens(result);

	system("pause");
}