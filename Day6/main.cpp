#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Point.hpp"
#include "Guard.hpp"
#include "Map.hpp"
#include <utility>

std::pair<Map, Guard>
parse_map(std::string filename)
{
	std::ifstream	file {filename};
	Guard			guard;
	Map				map;

	std::string	line;
	for (int x = 0, y = 0; file >> line; ++y)
	{
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
	std::pair	input = parse_map("example.txt");
	Map			map = input.first;
	Guard		guard = input.second;
	std::vector<Point>	path;
	map.print();
	while (guard.in_map())
	{
		guard.move(map);
		// map.print();
		path.push_back(guard.get_pos());
	}
}