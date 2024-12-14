#include <iostream>
#include "Map.hpp"
#include "Point.hpp"

Map::Map():
layout{0},
width(11),
height(7)
{ };

Map::Map(int width, int height):
layout{0},
width(width),
height(height)
{ };

void	Map::increment_grid(Point pos)
{
	++layout[pos.y][pos.x];
}

void	Map::visualize()
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			if (layout[y][x] == 0)
			{
				std::cout << '.';
			}
			else
			{
				std::cout << layout[y][x];
			}
		}
		std::cout << '\n';
	}
}