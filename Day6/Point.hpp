#ifndef POINT_HPP
#define POINT_HPP

class Point
{
public:
	int	x;
	int	y;
public:
	Point(/* args */) = default;
	Point(int, int);
	Point(const Point&) = default;
	~Point() = default;
};

#endif