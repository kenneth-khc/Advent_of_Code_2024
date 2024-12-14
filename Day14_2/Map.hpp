#ifndef MAP_HPP
#define MAP_HPP

#include <array>
#include <fstream>
#include "Point.hpp"

class Map
{
public:
	Map();
	Map(int, int);
	void	increment_grid(Point);
	void	visualize();
	void	visualize(std::ofstream&);
	std::array<std::array<int, 101>, 103>	layout;

	int	width;
	int height;
};

#endif
