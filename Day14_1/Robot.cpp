#include "Robot.hpp"
#include "Map.hpp"

Robot::Robot(Point pos, Point velo):
pos(pos),
velocity(velo)
{ } ;

void	Robot::move(Map& map)
{
	Point	new_pos = pos;
	new_pos.x += velocity.x;
	new_pos.y += velocity.y;
	if (new_pos.y >= map.height)
	{
		new_pos.y %= map.height;
	}
	else if (new_pos.y < 0)
	{
		new_pos.y += map.height;
	}
	if (new_pos.x >= map.width)
	{
		new_pos.x %= map.width;
	}
	else if (new_pos.x < 0)
	{
		new_pos.x += map.width;
	}
	pos = new_pos;
}