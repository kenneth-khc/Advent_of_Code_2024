#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "Point.hpp"
#include "Map.hpp"

class Robot
{
public:
	Robot() = default;
	Robot(Point, Point);
	void	move(Map& map);

	Point	pos;
	Point	velocity;

};

#endif