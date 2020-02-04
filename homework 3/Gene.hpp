#include <iostream>

class Gene
{
public:

	Gene();
	Gene(float, float);
	Gene& operator=(const Gene&);

	int getX() const;
	int getY() const;

	void set(int, int);
	void setX(int);
	void setY(int);

	friend std::ostream& operator<<(std::ostream&, const Gene&);
	friend bool operator<(const Gene& left, const Gene& right);

private:
	double x;
	double y;
};
