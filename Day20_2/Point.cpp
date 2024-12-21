#include <cmath>
#include "Point.hpp"

Point::Point(int x, int y): x(x), y(y) { };

bool	Point::operator==(const Point& other) const
{
	return this->x == other.x && this->y == other.y;
}

bool	Point::operator!=(const Point& other) const
{
	return !(*this == other);
}

bool	Point::operator<(const Point& other) const
{
	if (this->y == other.y)
	{
		return this->x < other.x;
	}
	else
	{
		return this->y < other.y;
	}
}

bool	Point::operator>(const Point& other) const
{
	if (this->y == other.y)
	{
		return this->x > other.x;
	}
	else
	{
		return this->y > other.y;
	}
}

Point&	Point::operator+=(const Point& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

Point	Point::operator+(const Point& rhs) const
{
	Point	lhs = *this;
	lhs += rhs;
	return lhs;
}

Point&	Point::operator-=(const Point& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	return *this;
}

Point	Point::operator-(const Point& rhs) const
{
	Point	lhs = *this;
	lhs -= rhs;
	return lhs;
}

std::ostream&	operator<<(std::ostream& os, const Point& point)
{
	os << "(" << point.x << ", " << point.y << ")";
	return os;
}

size_t	Point::Hasher::operator()(const Point& p) const
{
	size_t	x_hash = std::hash<int>()(p.x);
	size_t	y_hash = std::hash<int>()(p.y) << 1;
	return x_hash ^ y_hash;
}

Point	Point::find_in(std::vector<std::vector<char>> grid, const char& c)
{
	for (size_t row = 0; row < grid.size(); ++row)
	{
		for (size_t column = 0; column < grid[row].size(); ++column)
		{
			if (grid[row][column] == c)
			{
				return Point(column, row);
			}
		}
	}
	return Point(-1, -1);
}

uint64_t	Point::manhattan_dist(const Point& p1, const Point& p2)
{
	Point	diff = p2 - p1;
	return std::abs(diff.x) + std::abs(diff.y);
}

int	Point::euclidean_dist(const Point& p1, const Point& p2)
{
	Point	diff = p2 - p1;
	return std::sqrt(std::pow(diff.x, 2) + std::pow(diff.y, 2));
}