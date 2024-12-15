#ifndef BOX_HPP
#define BOX_HPP

#include "Point.hpp"

class Box
{
public:
	Box() = default;
	Box(int, int);
	Box(const Point&, const Point&);
	~Box() = default;

	Point	left;
	Point	right;
};

#endif