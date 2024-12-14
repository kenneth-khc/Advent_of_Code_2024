#ifndef MAP_HPP
#define MAP_HPP

#include <array>
#include "Point.hpp"

class Map
{
public:
	Map();
	Map(int, int);
	void	increment_grid(Point);
	void	visualize();
	std::array<std::array<int, 101>, 103>	layout;

	int	width;
	int height;
};

#endif