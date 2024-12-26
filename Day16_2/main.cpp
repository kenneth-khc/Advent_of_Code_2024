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
	(void)paths;
	if (curr == end)
	{
		++steps;
		return ;
	}
	else
	{
		++steps;
		// for (auto& predecessor : paths[curr])
		// {
		// 	count_steps_through_paths(predecessor, end, steps, paths);
		// }
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

#if 0
void	count_steps_through_paths(Node curr, Node start, int& steps, Paths& paths)
{
	++steps;
	if (curr.coords == start.coords)
	{
		std::cout << steps << " steps!\n";
		return ;
	}
	else
	{
		// for (auto& path : paths[curr])
		// {
			count_steps_through_paths(paths[curr][0], start, steps, paths);
		// }
		// if (paths[curr].size() == 2)
		// 	count_steps_through_paths(paths[curr][1], start, steps, paths);
	}
}
#endif

int main()
{
	Map		map = parse_map("example.txt");
	Point<>	exit_pos = Point<>::find_in(map, 'E');
	Point<>	reindeer_pos = Point<>::find_in(map, 'S');

	Paths	paths = dijkstra(map, reindeer_pos, exit_pos);

	std::vector<Path>	valid_paths {};
	if (paths.find({reindeer_pos}) != paths.end())
	{
		for (auto& path : paths[{reindeer_pos}])
		{
			valid_paths.push_back(path);
		}
	}

	int		steps {0};
	// while (curr.coords != reindeer_pos)
	// {
	// 	++steps;
	// 	std::cout << "(" << curr.coords.x << ", " << curr.coords.y << ")\n";
	// 	curr = paths[curr][0];
	// }
	// count_steps_through_paths(curr, reindeer_pos, steps, paths);
	// // count_steps_through_paths(pos, reindeer_pos, steps, paths);
	print(steps);
	return 0;
}
