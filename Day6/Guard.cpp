#include <vector>
#include <string>
#include <iostream>
#include "Guard.hpp"


Guard::Guard(int x, int y):
pos(x, y),
unique_tiles_travelled{0},
step_x{0},
step_y{-1},
escaped{false}
{ }

void    Guard::move(Map& map)
{
	size_t	new_x = pos.x + step_x;
	size_t	new_y = pos.y + step_y;

	if (new_y >= map.get_height() ||
		new_x >= map.get_width())
	{
		map.print();
		std::cout << "Guard exits map at " << "x:" << pos.x << " y:" << pos.y << std::endl;
		std::cout << "Guard has travelled " << unique_tiles_travelled
				  << " unique tiles" << std::endl;
		escaped = true;
		return ;
	}
	std::cout << "NY: " << new_y << " NX: " << new_x << std::endl;
	if (map(new_x, new_y) == '#')
	{
		change_direction();
	}
	else if (map(new_x, new_y) == 'X')
	{
		pos.x = new_x;
		pos.y = new_y;
	}
	else if (map(new_x, new_y) == '.')
	{
		if (map(pos.x, pos.y) == '^')
		{
			map.mark(pos.x, pos.y);
			++unique_tiles_travelled;
		}
		map(new_x, new_y) = 'X';
		++unique_tiles_travelled;
		map.mark(new_x, new_y);
		pos.x = new_x;
		pos.y = new_y;
	}
}

void	Guard::change_direction()
{
	// moving up
	if (step_x == 0 && step_y == -1)
	{
		step_x = +1;
		step_y = 0;
	}
	// moving right
	else if (step_x == +1 && step_y == 0)
	{
		step_x = 0;
		step_y = +1;
	}
	// moving down
	else if (step_x == 0 && step_y == +1)
	{
		step_x = -1;
		step_y = 0;
	}
	// moving left
	else
	{
		step_x = 0;
		step_y = -1;
	}
}

Point	Guard::get_pos() const
{
	return pos;
}

bool	Guard::in_map() const
{
	return !escaped;
}