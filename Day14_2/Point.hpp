#ifndef POINT_HPP
#define POINT_HPP

struct Point
{
	int	x;
	int	y;
	Point() = default;
	Point(int x, int y): x(x), y(y) { }
};

#endif