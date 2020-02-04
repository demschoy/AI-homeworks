#include <iostream>
#include <vector>

#define DIMENSION 3
#define MAX_TREE_DEPTH 8
#define MAX_MOVE 'x'
#define MIN_MOVE 'o'
#define EMPTY_CELL '_'
#define MAX_WINNING_VALUE 10
#define MIN_WINNING_VALUE -10
#define TIE_VALUE 0
#define COMPUTER 'c'
#define PLAYER 'y'

using Board = std::vector<std::vector<char>>;

struct Move
{
	int x;
	int y;
	
	Move(int _x, int _y)
		: x(_x), y(_y) {	}
};

void printBoard(Board state)
{
	std::cout << " ------------- \n";
	for (int row = 0; row < DIMENSION; row++)
	{
		for (int col = 0; col < DIMENSION; col++)
		{
			std::cout << " | " << state[row][col];

			if (col == 2)
			{
				std::cout << " |";
			}
		}
		std::cout << "\n ------------- \n";

	}
	std::cout << std::endl;
}

int evaluateRowWinner(Board state, int depth)
{
	int result = TIE_VALUE;
	for (int row = 0; row < DIMENSION; row++)
	{
		if (state[row][0] == state[row][1] && state[row][1] == state[row][2])
		{
			if (state[row][0] == MAX_MOVE)
			{
				result = MAX_WINNING_VALUE - depth;
			}
			else if (state[row][0] == MIN_MOVE)
			{
				result = MIN_WINNING_VALUE + depth;
			}
		}
	}
	return result;
}

int evaluateColumnWinner(Board state, int depth)
{
	int result = TIE_VALUE;
	for (int col = 0; col < DIMENSION; col++)
	{
		if (state[0][col] == state[1][col] && state[1][col] == state[2][col])
		{
			if (state[0][col] == MAX_MOVE)
			{
				result = MAX_WINNING_VALUE - depth;
			}
			else if (state[0][col] == MIN_MOVE)
			{
				result = MIN_WINNING_VALUE + depth;
			}
		}
	}
	return result;
}

int evaluateDiagonalWinner(Board state, int depth)
{
	int result = TIE_VALUE;

	if (state[0][0] == state[1][1] && state[1][1] == state[2][2])
	{
		if (state[0][0] == MAX_MOVE)
		{
			result = MAX_WINNING_VALUE - depth;
		}
		else if (state[0][0] == MIN_MOVE)
		{
			result = MIN_WINNING_VALUE + depth;
		}
	}
	else if (state[0][2] == state[1][1] && state[1][1] == state[2][0])
	{
		if (state[0][2] == MAX_MOVE)
		{
			result = MAX_WINNING_VALUE;
		}
		else if (state[0][2] == MIN_MOVE)
		{
			result = MIN_WINNING_VALUE;
		}
	}
	return result;
}

int evaluateBoard(Board state, int depth)
{
	int result = evaluateRowWinner(state, depth);
	if (result == TIE_VALUE)
	{
		result = evaluateColumnWinner(state, depth);
		if (result == TIE_VALUE)
		{
			result = evaluateDiagonalWinner(state, depth);
		}
	}
	return result;
}

bool isBoardFilled(Board state)
{
	for (int row = 0; row < DIMENSION; row++)
	{
		for (int col = 0; col < DIMENSION; col++)
		{
			if (state[row][col] != MAX_MOVE && state[row][col] != MIN_MOVE)
			{
				return false;
			}
		}
	}
	return true;
}

void announcePlayer(char playerSymbol)
{
	std::cout << "The player is the ";
	switch (playerSymbol)
	{
	case COMPUTER: std::cout << "COMPUTER\n"; break;
	case PLAYER: std::cout << "PLAYER\n"; break;
	}
	std::cout << "Computer plays with the " << MIN_MOVE << std::endl
		<< "You play with the " << MAX_MOVE << std::endl;
}

void announceWinner(int boardResult)
{
	switch (boardResult)
	{
	case TIE_VALUE: std::cout << "It is a draw\n"; break;
	case MAX_WINNING_VALUE: std::cout << "The winner is YOU\n"; break;
	case MIN_WINNING_VALUE: std::cout << "The winner is COMPUTER\n"; break;
	}
}

char choosePlayer()
{
	char playerSymbol;
	std::cout << "TIC TAC TOE" << std::endl
		<< "Please enter first player - c(computer) or y(you)" << std::endl;
	std::cin >> playerSymbol;

	while (playerSymbol != PLAYER && playerSymbol != COMPUTER)
	{
		std::cout << "Please enter a valid player identifier - c(computer) or y(you)" << std::endl;
		std::cin >> playerSymbol;
	}

	return playerSymbol;
}

std::vector<std::pair<Board, Move>> findSuccessorsOf(Board state, char move)
{
	std::vector<std::pair<Board, Move>> successors;
	for (int row = 0; row < DIMENSION; row++)
	{
		for (int col = 0; col < DIMENSION; col++)
		{
			if (state[row][col] == EMPTY_CELL)
			{
				state[row][col] = move;
				Move nextMove = Move(row, col);
				std::pair<Board, Move> nextState(state, nextMove);
				successors.push_back(nextState);
				state[row][col] = EMPTY_CELL;
			}
		}
	}
	return successors;
}

void makeMove(Board& state, Move coordinates, char move)
{
	if (state[coordinates.x][coordinates.y] == EMPTY_CELL)
	{
		state[coordinates.x][coordinates.y] = move;
	}
}

void makePlayerMove(Board& state)
{
	int row, column;
	std::cout << "Please enter coordinates of your move" << std::endl;
	std::cin >> row >> column;

	Move next(row, column);
	makeMove(state, next, MAX_MOVE);
}

int max(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	return b;
}

int min(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	return b;
}

int maxValue(Board, int);
std::pair<int, Move> minValue(Board, int);

int maxValue(Board state, int depth)
{
	int boardScore = evaluateBoard(state, depth);
	if (depth == MAX_TREE_DEPTH || boardScore > 0 || boardScore < 0 || isBoardFilled(state))
	{
		return boardScore;
	}
	
	int value = INT32_MIN;
	std::vector<std::pair<Board, Move>> successors = findSuccessorsOf(state, MAX_MOVE);
	for (auto nextState : successors)
	{
		value = max(value, (minValue(nextState.first, depth+1)).first);
	}
	return value;
}

std::pair<int, Move> minValue(Board state, int depth)
{
	int boardScore = evaluateBoard(state, depth);
	if (depth == MAX_TREE_DEPTH || boardScore >0 || boardScore <0 || isBoardFilled(state))
	{
		return std::pair<int, Move>(boardScore, Move(0,0));
	}

	int value = INT32_MAX;
	std::vector<std::pair<Board, Move>> successors = findSuccessorsOf(state, MIN_MOVE);
	std::pair<int, Move> valueMove(value, Move(0,0));
	for (auto nextState : successors)
	{
		value = maxValue(nextState.first, depth+1);
		if (value < valueMove.first)
		{
			valueMove.first = value;
			valueMove.second = nextState.second;
		}
	}
	return valueMove;
}

void minimaxAlgorithm(Board& state, int depth)
{
	int boardScore = evaluateBoard(state, depth);
	if (boardScore >0 || boardScore <0 || isBoardFilled(state))
	{
		return;
	}

	std::pair<int, Move> valueMove = minValue(state, depth);
	makeMove(state, valueMove.second, MIN_MOVE);
}

int maxAlphaBetaValue(Board, int, int, int);
std::pair<int, Move> minAlphaBetaValue(Board, int, int, int);

int maxAlphaBetaValue(Board state, int depth, int alpha, int beta)
{
	int boardScore = evaluateBoard(state, depth);
	if (depth == MAX_TREE_DEPTH || boardScore<0 || boardScore >0 || isBoardFilled(state))
	{
		return boardScore;
	}

	int value = INT32_MIN;
	std::vector<std::pair<Board, Move>> successors = findSuccessorsOf(state, MAX_MOVE);
	for (auto nextState : successors)
	{
		value = max(value, (minAlphaBetaValue(nextState.first, depth + 1, alpha, beta)).first);
		if (value >= beta)
		{
			return value;
		}
		alpha = max(alpha, value);
	}
	return value;
}

std::pair<int, Move> minAlphaBetaValue(Board state, int depth, int alpha, int beta)
{
	int boardScore = evaluateBoard(state, depth);
	if (depth == MAX_TREE_DEPTH || boardScore >0 || boardScore <0 || isBoardFilled(state))
	{
		return std::pair<int, Move>(boardScore, Move(0,0));
	}

	int value = INT32_MAX;
	std::vector<std::pair<Board, Move>> successors = findSuccessorsOf(state, MIN_MOVE);
	std::pair<int, Move> valueMove(value, Move(0, 0));
	for (auto nextState : successors)
	{
		value = maxAlphaBetaValue(nextState.first, depth + 1, alpha, beta);
		if (value < valueMove.first)
		{
			valueMove.first = value;
			valueMove.second = nextState.second;
		}
		if (valueMove.first <= alpha)
		{
			return valueMove;
		}
		beta = min(valueMove.first, beta);
	}
	return valueMove;
}

void alphaBetaSearchAlgorithm(Board& state, int depth)
{
	int boardScore = evaluateBoard(state, depth);
	if (boardScore < 0 || boardScore > 0 || isBoardFilled(state))
	{
		return;
	}

	int alpha = INT32_MIN;
	int beta = INT32_MAX;
	std::pair<int, Move> valueMove = minAlphaBetaValue(state, depth, alpha, beta);
	makeMove(state, valueMove.second, MIN_MOVE);
}

void ticTacToe(Board& initialBoard, char playerSymbol)
{
	int boardScore = evaluateBoard(initialBoard, 0);
	int depth = 0;
	while (!isBoardFilled(initialBoard) && boardScore > 0 || boardScore < 0)
	{
		if (playerSymbol == COMPUTER)
		{
			//minimaxAlgorithm(initialBoard, depth);
			alphaBetaSearchAlgorithm(initialBoard, depth);
			printBoard(initialBoard);
			playerSymbol = PLAYER;
		}
		else if (playerSymbol == PLAYER)
		{
			makePlayerMove(initialBoard);
			printBoard(initialBoard);
			playerSymbol = COMPUTER;
		}
		boardScore = evaluateBoard(initialBoard, depth);
	}
	
	announceWinner(boardScore);
}

int main()
{
	Board initialBoard = Board(DIMENSION);
	for (int i = 0; i < DIMENSION; i++)
	{
		initialBoard[i].assign(DIMENSION, EMPTY_CELL);
	}

	char playerSymbol = choosePlayer();
	announcePlayer(playerSymbol);

	ticTacToe(initialBoard, playerSymbol);

	system("pause");
	return 0;
}