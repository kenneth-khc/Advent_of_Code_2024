#include <iostream>
#include "Map.hpp"

void    Map::add(std::string line)
{
    layout.push_back(line);
    width = line.size();
    height++;
}

void    Map::print()
{
    for (const auto& row : layout)
    {
        std::cout << row << '\n';
    }
}

void    Map::mark(size_t x, size_t y)
{
    if (y >= height || x >= width)
    {
        return ;
    }
    layout[y][x] = 'X';
}

std::string&	Map::operator[](size_t idx)
{
    return layout[idx];
}

size_t	Map::get_width()
{
	return width;
}

size_t	Map::get_height()
{
	return height;
}