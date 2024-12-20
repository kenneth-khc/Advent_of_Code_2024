#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <vector>
#include <cstddef>
#include <functional>

using std::size_t;

class Point
{
public:
    Point() = default;
    ~Point() = default;
    Point(int x, int y): x(x), y(y) { };

    bool	operator==(const Point& other) const
	{
		return this->x == other.x && this->y == other.y;
	}

	bool	operator<(const Point& other) const
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
	bool	operator>(const Point& other) const
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



	struct Hasher
	{
		size_t	operator()(const Point& p) const
		{
			size_t	x_hash = std::hash<int>()(p.x);
			size_t	y_hash = std::hash<int>()(p.y) << 1;
			return x_hash ^ y_hash;
		}
	};

	static Point	find_in(std::vector<std::vector<char>> grid, const char& c)
	{
		for (size_t y = 0; y < grid.size(); ++y)
		{
			for (size_t x = 0; x < grid[y].size(); ++x)
			{
				if (grid[y][x] == c)
				{
					return Point(x, y);
				}
			}
		}
		return Point(-1, -1);
	}

    int x;
    int y;
};

std::ostream&	operator<<(std::ostream& os, const Point& point)
{
	os << "(" << point.x << ", " << point.y << ")";
	return os;
}

#endif