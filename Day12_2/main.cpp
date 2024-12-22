#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <deque>
#include "Point.hpp"
#include "Plot.hpp"

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

bool	in_map(Map& map, const Point& p)
{
	size_t	x = p.x;
	size_t	y = p.y;
	return y < map.size() && x < map[y].size();
}

bool	in_map(Map& map, Plot& p)
{
	size_t	x = p.x;
	size_t	y = p.y;
	return y < map.size() && x < map[y].size();
}

std::vector<Plot>	get_neighbours(Map& map, Plot& p)
{
	static const std::vector<Plot>	diffs =
	{
		{0, +1},
		{0, -1},
		{+1, 0},
		{-1, 0}
	};
	std::vector<Plot>	neighbours {};
	for (const auto& diff : diffs)
	{
		int	x = p.x + diff.x;
		int	y = p.y + diff.y;
		if (in_map(map, Plot(x, y)) && map[y][x] == p.symbol)
		{
			neighbours.push_back(Plot(x, y, p.symbol));
		}
	}
	return neighbours;
}

std::vector<Plot>	get_other_region_neighbours(Map& map, Plot& p)
{
	static const std::vector<Plot>	diffs =
	{
		{0, +1},
		{0, -1},
		{+1, 0},
		{-1, 0}
	};
	std::vector<Plot>	neighbours {};
	for (const auto& diff : diffs)
	{
		int	x = p.x + diff.x;
		int	y = p.y + diff.y;
		if (in_map(map, Plot(x, y)) && map[y][x] != p.symbol)
		{
			neighbours.push_back(Plot(x, y, map[y][x]));
		}
	}
	return neighbours;
}

void	check_fencing(Plot& visiting, std::vector<Plot>& neighbours)
{
	static const std::vector<Plot>	diffs =
	{
		{0, +1},
		{0, -1},
		{+1, 0},
		{-1, 0}
	};
	for (const auto& neighbour : neighbours)
	{
		Point	diff = neighbour - visiting;
		if (diff == diffs[NORTH])
			visiting.fenced[NORTH] = false;
		else if (diff == diffs[SOUTH])
			visiting.fenced[SOUTH] = false;
		else if (diff == diffs[EAST])
			visiting.fenced[EAST] = false;
		else if (diff == diffs[WEST])
			visiting.fenced[WEST] = false;
	}
}

void	clear_region_on_map(Map& map, std::vector<Plot>& region)
{
	for (const auto& plot : region)
	{
		map[plot.y][plot.x] = 0;
	}
}

using HashSet = std::unordered_set<Plot,Point::Hasher>;
std::vector<Plot>	find_region(Map& map, Plot p)
{
	std::vector<Plot>	region {};
	std::deque<Plot>	searching {};
	HashSet				visited {};
	searching.push_back(p);
	while (!searching.empty())
	{
		Plot	visiting = searching.front();
		searching.pop_front();
		std::vector<Plot>	neighbours = get_neighbours(map, visiting);
		check_fencing(visiting, neighbours);
		for (auto& neighbour : neighbours)
		{
			if (!visited.contains(neighbour)
			 && std::find(searching.begin(), searching.end(), neighbour) == searching.end())
			{
				searching.push_back(neighbour);
			}
		}
		if (map[visiting.y][visiting.x] == p.symbol)
		{
			region.push_back(visiting);
			visited.insert(visiting);
			// map[visiting.y][visiting.x] = 0;
		}
	}
	return region;
}

uint64_t	get_side_diff(std::array<bool,4> fences1, std::array<bool,4> fences2)
{
	uint64_t	diff {0};
	for (size_t i = 0; i < 4; ++i)
	{
		if (fences1[i] == true && fences2[i] == false)
			++diff;
	}
	return diff;
}

std::vector<Plot>	find_edges(Map& map, std::vector<Plot>& region)
{
	std::vector<Plot>	edges {};

	for (auto it = region.begin(); it != region.end(); ++it)
	{
		std::vector<Plot>	neighbours = get_other_region_neighbours(map, *it);
		if (!neighbours.empty())
		{
			edges.push_back(*it);
		}
	}
	return edges;
}

uint64_t	calculate_region_price(Map& map, Point p)
{
	std::vector<Plot>	region = find_region(map, Plot(p, map[p.y][p.x]));
	std::vector<Plot>	edges = find_edges(map, region);
	uint64_t	total_area = region.size();
	uint64_t	total_sides {0};

	std::array<bool,4>	mask = {false, false, false, false};
	Plot*	start = &edges[0];
	if (start->fenced != mask)
	{
		mask = start->fenced;
		// total_sides += get_side_diff(start->fenced, curr->fenced);
		++total_sides;
	}
	Plot*	curr = &edges[1];
	for (int i = 1; curr != start; ++i %= edges.size())
	{
		curr = &edges[i]; if(curr == start) break;
		if (curr->fenced == mask) continue;
		else if (curr->fenced == std::array<bool,4> {false, false, false, false}) continue;
		else
		{
			// total_sides += get_side_diff(mask, curr->fenced);
			++total_sides;
			mask = curr->fenced;
		}
	}

	std::cout << "Found " << total_sides << " total sides for region " << region[0].symbol << '\n';

	clear_region_on_map(map, region);
	return total_area * total_sides;
}

int	main()
{
	Map	map = parse("example.txt");

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