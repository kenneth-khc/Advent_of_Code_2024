#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <deque>
#include "Point.hpp"
#include <queue>

using Map = std::vector<std::vector<char>>;

Map	parse(const std::string& filename)
{
	Map	map {};

	std::ifstream	file {filename};
	std::string		line;
	while (file >> line)
	{
		map.push_back(std::vector(line.begin(), line.end()));
	}
	return map;
}

void	print(std::vector<Point>& region)
{
	for (auto& plant : region)
	{
		std::cout << plant << '\n';
	}
}

bool	in_map(Map& map, Point p)
{
	size_t	x = p.x;
	size_t	y = p.y;
	return y < map.size() && x < map[y].size();
}

std::vector<Point>	get_neighbours(Point& p)
{
	return
	{
		{p.x, p.y-1},
		{p.x, p.y+1},
		{p.x-1, p.y},
		{p.x+1, p.y}
	};
}

using HashSet = std::unordered_set<Point,Point::Hasher>;
std::vector<Point>	find_region(Map& map, Point& p)
{
	const char	letter = map[p.y][p.x];
	std::vector<Point>	region {};
	std::deque<Point>	searching {};
	HashSet				visited {};
	searching.push_back(p);
	while (!searching.empty())
	{
		Point	visiting = searching.front();
		searching.pop_front();
		if (map[visiting.y][visiting.x] == letter)
		{
			region.push_back(visiting);
			visited.insert(visiting);
			map[visiting.y][visiting.x] = 0;
		}
		std::vector<Point>	neighbours = get_neighbours(visiting);
		for (auto& neighbour : neighbours)
		{
			if (in_map(map, neighbour)
			 && map[neighbour.y][neighbour.x] == letter
			 && !visited.contains(neighbour)
			 && std::find(searching.begin(), searching.end(), neighbour) == searching.end())
			{
				searching.push_back(neighbour);
			}
		}
	}
	return region;
}
uint64_t	calculate_region_price(Map& map, Point p)
{
	std::vector<Point> region = find_region(map, p);
	uint64_t	total_area = region.size();
	uint64_t	total_perimeter {0};
	for (auto& plant : region)
	{
		int	perimeter {4};
		std::vector<Point>	neighbours = get_neighbours(plant);
		for (auto& neighbour : neighbours)
		{
			if (std::find(region.begin(), region.end(), neighbour) != region.end())
			{
				--perimeter;
			}
		}
		total_perimeter += perimeter;
	}
	return total_area * total_perimeter;
}

int	main()
{
	Map	map = parse("input.txt");

	uint64_t	total_price {0};
	for (size_t row = 0; row < map.size(); ++row)
	{
		for (size_t column = 0; column < map[row].size(); ++column)
		{
			if (map[row][column] != 0)
			{
				total_price += calculate_region_price(map, Point(column, row));
			}
		}
	}
	std::cout << "Total price is " << total_price << std::endl;
}