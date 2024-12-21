#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <unordered_map>
#include <vector>
#include <deque>
#include <limits>
#include <iterator>
#include <algorithm>
#include <functional>
#include "Point.hpp"

using Map = std::vector<std::vector<char>>;

Map	parse(const std::string& filename)
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

void	print(Map& map)
{
	for (size_t y = 0; y < map.size(); ++y)
	{
		for (size_t x = 0; x < map[y].size(); ++x)
		{
			std::cout << map[y][x];
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

std::vector<Point>	get_path(const Map& map, const Point& start, const Point& end)
{
	std::vector<Point>	path {};
	std::set<Point>		visited {};
	Point				next {start};
	std::function<bool(const Point&)>	is_new_walkable = 
	[&](const Point& p)
	{
		return map[p.y][p.x] != '#'
		&& std::find(visited.begin(), visited.end(), p) == visited.end();
	};

	visited.insert(next);
	path.push_back(next);
	while (next != end)
	{
		std::vector<Point>	neighbours =
		{
			Point{next.x, next.y - 1},
			Point{next.x, next.y + 1},
			Point{next.x - 1, next.y},
			Point{next.x + 1, next.y},
		};
		auto it = std::find_if(neighbours.begin(), neighbours.end(), is_new_walkable);
		next = *it;
		visited.insert(next);
		path.push_back(next);
	}
	return path;
}

std::set<Point>	get_walkables(Map& map)
{
	std::set<Point>	walkables {};

	for (size_t row = 0; row < map.size(); ++row)
	{
		for (size_t column = 0; column < map[row].size(); ++column)
		{
			if (map[row][column] != '#')
			{
				walkables.insert(Point(column, row));
			}
		}
	}
	return walkables;
}

using Cheats = std::map<int, int>;

std::map<Point,int>	get_distances_from_start(std::set<Point>& walkables, Point& start)
{
	std::map<Point,int>	distances {};

	for (auto it = walkables.begin(); it != walkables.end(); ++it)
	{
		const Point&	tile = *it;
		const int		distance = Point::manhattan_dist(start, tile);
		distances.emplace(tile, distance);
	}
	return distances;
}

std::map<Point,int>	get_distances_to_end(std::set<Point>& walkables, Point& end)
{
	std::map<Point,int>	distances {};

	for (auto it = walkables.begin(); it != walkables.end(); ++it)
	{
		const Point&	tile = *it;
		const int		distance = Point::manhattan_dist(tile, end);
		distances.emplace(tile, distance);
	}
	return distances;
}

// A map of distances between a pair of points
using DistanceMap = std::map<std::pair<Point,Point>,int>;
DistanceMap	get_distances_between_points(std::set<Point>& walkables)
{
	DistanceMap	distances {};

	for (auto it = walkables.begin(); it != walkables.end(); ++it)
	{
		const Point&	p1 = *it;
		for (auto it2 = walkables.begin(); it2 != walkables.end(); ++it2)
		{
			// if (it2 == it) continue;
			const Point&	p2 = *it2;
			const int		distance = Point::manhattan_dist(p1, p2);
			distances.emplace(std::make_pair(p1, p2), distance);
			// distances.insert({std::make_pair(p1, p2), distance});
		}
	}
	return distances;
}

using StepTable = std::map<std::pair<Point,Point>,int>;
StepTable	get_steps_between_points(std::vector<Point>& path)
{
	StepTable	step_table {};

	for (auto it = path.begin(); it != path.end(); ++it)
	{
		for (auto itt = it; itt != path.end(); ++itt)
		{
			if (itt == it) continue ;
			const Point&	src = *it;
			const Point&	dest = *itt;
			const int		steps = std::distance(it, itt);
			step_table.emplace(std::make_pair(src, dest), steps);
		}
	}
	return step_table;
}


int	cheat(Map& map, const StepTable& steptable, const std::vector<Point>& path, std::unordered_map<Point,int,Point::Hasher>& index_table, Point& start, Point& end, const Point& cheat_start, const Point& cheat_end)
{(void)start;(void)end;(void)map;
	std::set<Point>		visited {};
	static const auto	path_begin = path.begin();
	static const auto	path_end = path.end();

	size_t	steps {0};
	// auto	itt = path_begin + index_table[cheat_start];
	const std::pair<Point,Point>	start_to_cheat_start {start, cheat_start};
	int	first_steps = steptable[start_to_cheat_start];
	const std::pair<Point,Point>	cheat_end_to_end {cheat_end, end};
	auto	cheat_start_it = path_begin + index_table[cheat_start];
	for (auto it = path_begin; it != cheat_start_it; ++it)
	{
		++steps;
	}
	// visited.insert(path_begin, itt);
	// auto	it = path_begin + index_table[cheat_end];
	auto	cheat_end_it = path_begin + index_table[cheat_end];
	for (auto it = cheat_end_it; it != path_end; ++it)
	{
		++steps;
	}
	return steps - 1;
	// return (visited.size() - 1);
}


bool	cheatable(std::unordered_map<Point,int,Point::Hasher>& index_table,
				  const Point& p1, const Point& p2)
{
	std::ptrdiff_t	p1_idx = index_table[p1];
	std::ptrdiff_t	p2_idx = index_table[p2];
	return p1_idx < p2_idx;
}

std::unordered_map<Point,int,Point::Hasher>	init_index_table(std::vector<Point>& path)
{
	std::unordered_map<Point,int,Point::Hasher>	table {};

	int	i {0};
	for (auto it = path.begin(); it != path.end(); ++it)
	{
		table.emplace(*it, i);
		++i;
	}
	return table;
}

int	main()
{
	Map				map = parse("input.txt");
	Point			start = Point::find_in(map, 'S');
	Point			end = Point::find_in(map, 'E');

	std::set<Point>	walkables = get_walkables(map);
	const int		initial_steps = walkables.size() - 1;
	std::vector<Point>	path = get_path(map, start, end);
	std::unordered_map<Point,int,Point::Hasher>	index_table = init_index_table(path);
	DistanceMap	distances = get_distances_between_points(walkables);
	std::map<int,int>	occurrences {};

	for (auto it = distances.begin(); it != distances.end(); ++it)
	{
		const std::pair<Point,Point> pair = it->first;
		const int&		dist = it->second;
		const Point&	p1 = pair.first;
		const Point&	p2 = pair.second;
		if (dist < 21 && cheatable(index_table, p1, p2))
		{
			// std::cout << "Can cheat from " << p1 << " to " << p2 << '\n';
			int steps = cheat(map, path, index_table, start, end, p1, p2) + dist;
			int steps_saved = initial_steps - steps;
			++occurrences[steps_saved];
			// std::cout << "Steps saved: " << steps_saved << '\n';
			// std::cout << "------------------------------\n";
		}
	}
	int	over_100ps_saved_count {0};
	for (auto it = occurrences.begin(); it != occurrences.end(); ++it)
	{
		const int&	time_saved = it->first;
		const int&	count = it->second;
		std::cout << count << " cheats that save " << time_saved << '\n';
		if (time_saved >= 100)
			++over_100ps_saved_count;
	}
	std::cout << over_100ps_saved_count << " cheats that saved over 100 picoseconds\n";
	// for (auto it = walkables.begin(); it != walkables.end(); ++it)
	// {
	// 	auto tile = *it;
	// 	std::cout << "At " << tile << " to " << end << ": " << Point::manhattan_dist(tile, end) << '\n';
	// }

	// print(map);
}