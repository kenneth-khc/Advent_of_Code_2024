#include <vector>
#include <string>
#include <iostream>
#include "Guard.hpp"


Guard::Guard(int x, int y):
x{x},
y{y},
unique_tiles_travelled{0},
step_x{0},
step_y{-1}
{ }

void    Guard::move(Map& map)
{
	size_t	new_x = x + step_x;
	size_t	new_y = y + step_y;

	if (new_y >= map.get_height() ||
		new_x >= map.get_width())
	{
		map.print();
		std::cout << "Guard exits map at " << "x:" << x << " y:" << y << std::endl;
		std::cout << "Guard has travelled " << unique_tiles_travelled
				  << " unique tiles" << std::endl;
		std::exit(0);
	}
	std::cout << "NY: " << new_y << " NX: " << new_x << std::endl;
	if (map[new_y][new_x] == '#')
	{
		change_direction();
	}
	else if (map[new_y][new_x] == 'X')
	{
		x = new_x;
		y = new_y;
	}
	else if (map[new_y][new_x] == '.')
	{
		if (map[y][x] == '^')
		{
			map[y][x] = 'X';
			++unique_tiles_travelled;
			map.mark(y, x);
		}
		map[new_y][new_x] = 'X';
		++unique_tiles_travelled;
		map.mark(new_x, new_y);
		x = new_x;
		y = new_y;
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