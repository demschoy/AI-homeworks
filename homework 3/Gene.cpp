#include "Gene.hpp"

Gene::Gene() : x(0), y(0) { }

Gene::Gene(float _x, float _y) : x(_x), y(_y) {	}

Gene& Gene::operator=(const Gene& other)
{
	if (this != &other)
	{
		x = other.x;
		y = other.y;
	}
	return *this;
}

int Gene::getX() const
{
	return x;
}

int Gene::getY() const
{
	return y;
}

void Gene::set(int newX, int newY)
{
	x = newX;
	y = newY;
}

void Gene::setX(int newX)
{
	x = newX;
}

void Gene::setY(int newY)
{
	y = newY;
}

std::ostream& operator<<(std::ostream& os, const Gene& gene)
{
	os << "(" << gene.x << ", " << gene.y << ")";
	return os;
}

bool operator<(const Gene& left, const Gene& right)
{
	bool first = left.x < right.x;
	bool second = left.x == right.x && left.y < right.y;
	return first || second;
}

