#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Guard.hpp"

int main()
{
    std::ifstream				file{"input.txt"};
	std::vector<std::string>	map;
	Guard						guard;

	for (int x = 0, y = 0; !file.eof(); y++)
	{
		std::string	line;
		std::getline(file, line);
		map.push_back(line);
		x = static_cast<int>(line.find('^'));
		if (static_cast<size_t>(x) != line.npos)
		{
			guard = Guard(x, y);
		}
	}
	while (1)
	{
		guard.move(map);
	}
}