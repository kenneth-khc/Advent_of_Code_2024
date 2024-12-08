#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Guard.hpp"
#include "Map.hpp"
#include <utility>

std::pair<Map, Guard>
parse_map(std::string filename)
{
	std::ifstream	file {filename};
	Guard			guard;
	Map				map;

	for (int x = 0, y = 0; !file.eof(); y++)
	{
		std::string	line;
		std::getline(file, line);
		map.add(line);
		x = static_cast<int>(line.find('^'));
		if (static_cast<size_t>(x) != line.npos)
		{
			guard = Guard(x, y);
		}
	}
	return std::make_pair(map, guard);
}

int main()
{
	std::pair	input = parse_map("input.txt");
	Map			map = input.first;
	Guard		guard = input.second;
	map.print();
	while (1)
	{
		guard.move(map);
		// map.print();
	}
}