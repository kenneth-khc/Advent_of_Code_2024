#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include "Point.hpp"
#include "dijkstra.hpp"

template<typename T>
void	print(T t)
{
	std::cout << t << '\n';
}

void	print(Map& map)
{
	for (size_t row = 0; row < map.size(); ++row)
	{
		for (size_t column = 0; column < map[column].size(); ++column)
		{
			std::cout << map[row][column];
		}
		std::cout << '\n';
	}
}

void	count_steps_through_paths(Point<>& curr, Point<>& end, int& steps, Paths& paths)
{
	if (curr == end)
	{
		++steps;
		return ;
	}
	else
	{
		++steps;
		for (auto& predecessor : paths[curr])
		{
			count_steps_through_paths(predecessor, end, steps, paths);
		}
	}
}

Map
parse_map(const std::string& filename)
{
    std::ifstream	file {filename};
	std::string		line;
    Map				map;

	while (file >> line)
	{
		map.push_back(std::vector(line.begin(), line.end()));
	}
	return map;
}

int main()
{
	Map			map = parse_map("example.txt");
	Point<>		exit_pos = Point<>::find_in(map, 'E');
	Point<>		reindeer_pos = Point<>::find_in(map, 'S');

	Paths	paths = dijkstra(map, reindeer_pos, exit_pos);

	Point<>	pos = exit_pos;
	int		steps {0};
	count_steps_through_paths(pos, reindeer_pos, steps, paths);
	print(steps);
	return 0;
}
