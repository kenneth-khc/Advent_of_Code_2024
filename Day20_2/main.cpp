#include <fstream>
#include <iostream>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <vector>
#include <limits>
#include <iterator>
#include <algorithm>
#include <functional>
#include <chrono>
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

using Point_HashSet = std::unordered_set<Point,Point::Hasher>;

/*	Get the path from the start to the end, with the indexes indicating how many
	steps it takes to get to that tile */
std::vector<Point>	get_path(const Map& map, const Point& start, const Point& end)
{
	std::vector<Point>	path {}; path.reserve(20000);
	Point_HashSet		visited {};
	Point				next {start};
	std::function<bool(const Point&)>	is_new_walkable = 
	[&](const Point& p)
	{
		return map[p.y][p.x] != '#'
		&& visited.find(p) == visited.end();
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

/* Get all the possible walkable tiles on the map */
Point_HashSet	get_walkables(Map& map)
{
	Point_HashSet	walkables {};

	for (size_t row = 0; row < map.size(); ++row)
	{
		for (size_t column = 0; column < map[row].size(); ++column)
		{
			if (map[row][column] != '#')
			{
				walkables.emplace(Point(column, row));
			}
		}
	}
	return walkables;
}

/*	A map of distances between a pair of points 
	We are only concerned about distances that are within 20 because
	those are the only possible cheats */
using Distance_Map = std::map<std::pair<Point,Point>,int>;
Distance_Map	get_distances_between_points(Point_HashSet& walkables)
{
	Distance_Map	distances {};

	Point_HashSet::iterator	end = walkables.end();
	for (auto it = walkables.begin(); it != end; ++it)
	{
		for (auto it2 = walkables.begin(); it2 != end; ++it2)
		{
			const Point&	p1 = *it;
			const Point&	p2 = *it2;
			if (p1 == p2) continue;

			const int	distance = Point::manhattan_dist(p1, p2);
			if (distance < 21)
			{
				distances.emplace(std::make_pair(p1, p2), distance);
			}
		}
	}
	return distances;
}

using PathIndex_HashMap = std::unordered_map<Point,int,Point::Hasher>;
int	count_non_cheat_path(PathIndex_HashMap& index_table, const Point& cheat_start, const Point& cheat_end)
{
	size_t	steps {0};
	steps += index_table[cheat_start];
	steps += index_table.size() - index_table[cheat_end];
	return steps - 1; // -1 because it counts the starting position but that isn't a step
}

bool	in_order(PathIndex_HashMap& index_table,
				  const Point& p1, const Point& p2)
{
	std::ptrdiff_t	p1_idx = index_table[p1];
	std::ptrdiff_t	p2_idx = index_table[p2];
	return p1_idx < p2_idx;
}

/*	A HashMap with a Point as a key and the amount of steps it takes
	to reach it as the value */
PathIndex_HashMap	init_index_table(std::vector<Point>& path)
{
	PathIndex_HashMap	table {};

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
	auto	clock_start = std::chrono::steady_clock::now();
	Map		map = parse("input.txt");
	Point	start = Point::find_in(map, 'S');
	Point	end = Point::find_in(map, 'E');

	Point_HashSet	walkables = get_walkables(map);
	std::cout << "Done getting all walkable tiles on the map...\n";

	std::vector<Point>	path = get_path(map, start, end);
	std::cout << "Done getting a straight path to the exit...\n";

	PathIndex_HashMap	index_table = init_index_table(path);
	Distance_Map		distances = get_distances_between_points(walkables);
	std::cout << "Done getting the distances between each pair of walkable points...\n";

	std::cout << "Calculating the savings of cheating...\n";
	std::map<int,int>	occurrences {};
	const int	initial_steps = walkables.size() - 1;
	for (auto it = distances.begin(); it != distances.end(); ++it)
	{
		const std::pair<Point,Point> pair = it->first;
		const int&		cheat_dist = it->second;
		const Point&	p1 = pair.first;
		const Point&	p2 = pair.second;
		if (in_order(index_table, p1, p2)) // we don't cheat backwards towards the start
		{
			const int	ethical_steps = count_non_cheat_path(index_table, p1, p2);
			const int	total_steps = ethical_steps + cheat_dist;
			const int	steps_saved = initial_steps - total_steps;
			++occurrences[steps_saved];
		}
	}
	int	over_100ps_saved_count {0};
	int	total_cheats {0};
	for (auto it = occurrences.begin(); it != occurrences.end(); ++it)
	{
		const int&	time_saved = it->first;
		const int&	count = it->second;

		std::cout << count << " cheats that save " << time_saved << " picoseconds\n";
		total_cheats += count;
		if (time_saved >= 100)
		{
			over_100ps_saved_count += count;
		}
	}
	std::cout << "Total cheats discovered: " << total_cheats << '\n';
	std::cout << over_100ps_saved_count << " cheats that saved over 100 picoseconds\n";
	auto	clock_end = std::chrono::steady_clock::now();
	auto	clock_diff = clock_end - clock_start;
	std::cout << "Time: " << std::chrono::duration<double, std::milli>(clock_diff).count()
			  << "ms" << std::endl;
	return 0;
}