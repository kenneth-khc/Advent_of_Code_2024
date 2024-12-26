#include <cmath>
#include "Point.hpp"

template<typename Scalar>
Point<Scalar>::Point(Scalar x, Scalar y):
x(x),
y(y)
{

}

template<typename Scalar>
bool	Point<Scalar>::operator==(const Point& other) const
{
	return this->x == other.x && this->y == other.y;
}

template<typename Scalar>
bool	Point<Scalar>::operator!=(const Point& other) const
{
	return !(*this == other);
}

template<typename Scalar>
bool	Point<Scalar>::operator<(const Point& other) const
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

template<typename Scalar>
bool	Point<Scalar>::operator>(const Point& other) const
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

template<typename Scalar>
Point<Scalar>&	Point<Scalar>::operator+=(const Point& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

template<typename Scalar>
Point<Scalar>	Point<Scalar>::operator+(const Point& rhs) const
{
	Point	lhs = *this;
	lhs += rhs;
	return lhs;
}

template<typename Scalar>
Point<Scalar>&	Point<Scalar>::operator-=(const Point& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	return *this;
}

template<typename Scalar>
Point<Scalar>	Point<Scalar>::operator-(const Point& rhs) const
{
	Point	lhs = *this;
	lhs -= rhs;
	return lhs;
}

template<typename Scalar>
std::ostream&	operator<<(std::ostream& os, const Point<Scalar>& point)
{
	os << "(" << point.x << ", " << point.y << ")";
	return os;
}

template<typename Scalar>
size_t	Point<Scalar>::Hasher::operator()(const Point& p) const
{
	size_t	x_hash = std::hash<int>()(p.x);
	size_t	y_hash = std::hash<int>()(p.y) << 1;
	return x_hash ^ y_hash;
}

template<typename Scalar>
size_t	Point<Scalar>::Hasher::operator()(const std::pair<Point,Point>& pair) const
{
	size_t	first_hash = Hasher::operator()(pair.first);
	size_t	second_hash = Hasher::operator()(pair.second);
	return first_hash ^ second_hash;
}

template<typename Scalar>
Point<Scalar>	Point<Scalar>::find_in(std::vector<std::vector<char>> grid, const char& c)
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

template<typename Scalar>
uint64_t	Point<Scalar>::manhattan_dist(const Point& p1, const Point& p2)
{
	Point	diff = p2 - p1;
	return std::abs(diff.x) + std::abs(diff.y);
}

template<typename Scalar>
int	Point<Scalar>::euclidean_dist(const Point& p1, const Point& p2)
{
	Point	diff = p2 - p1;
	return std::sqrt(std::pow(diff.x, 2) + std::pow(diff.y, 2));
}