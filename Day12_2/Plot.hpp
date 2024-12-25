#ifndef PLOT_HPP
#define PLOT_HPP

#include <array>
#include "Point.hpp"

enum	Direction
{
	NORTH = 0,
	SOUTH = 1,
	EAST = 2,
	WEST = 3
};

class	Plot : public Point
{
public:
	Plot() = default;
	~Plot() = default;
	Plot(const Plot&) = default;
	Plot(int,int);
	Plot(int,int,char);
	Plot(const Point&,char);
	bool	operator==(const Plot&) const;
	bool	operator<(const Plot&) const;
	bool	operator>(const Plot&) const;
	Plot	operator-(const Plot&) const;
	Plot&	operator=(const Plot&) = default;

	char			type;
	std::array<bool,4>	fenced;
};

Plot::Plot(int x, int y):
Point {x, y},
type {0},
fenced {true, true, true, true}
{ }

Plot::Plot(int x, int y, char c):
Point {x, y},
type {c},
fenced {true, true, true, true}
{ }

Plot::Plot(const Point& p, char c):
Point {p},
type {c},
fenced {true, true, true, true}
{ }

bool	Plot::operator==(const Plot& other) const
{
	return Point::operator==(other);
}

bool	Plot::operator<(const Plot& other) const
{
	return Point::operator<(other);
}

bool	Plot::operator>(const Plot& other) const
{
	return Point::operator>(other);
}

Plot	Plot::operator-(const Plot& rhs) const
{
	return Plot(this->x - rhs.x, this->y - rhs.y);
}

#endif