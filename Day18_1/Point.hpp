#ifndef POINT_HPP
#define POINT_HPP

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

	struct Hasher
	{
		size_t	operator()(const Point& p) const
		{
			size_t	x_hash = std::hash<int>()(p.x);
			size_t	y_hash = std::hash<int>()(p.y) << 1;
			return x_hash ^ y_hash;
		}
	};

    int x;
    int y;
};

#endif