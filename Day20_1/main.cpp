#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <unordered_map>
#include <vector>
#include <deque>
#include <limits>
#include <iterator>
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

using Set = std::unordered_map<Point,int,Point::Hasher>;
using Paths = std::unordered_map<Point,Point,Point::Hasher>;

Set	construct_set(Map& map)
{
	Set	walkables;

	for (size_t y = 0; y < map.size(); ++y)
	{
		for (size_t x = 0; x < map[y].size(); ++x)
		{
			if (map[y][x] != '#')
			{
				Point	point {(int)x, (int)y};
				walkables.insert({point, std::numeric_limits<int>::max()});
			}
		}
	}
	return walkables;
}

bool	should_check(Set& unvisited)
{
	bool	empty = unvisited.empty();
	bool	reachable = false;
	for (auto it = unvisited.begin(); it != unvisited.end(); ++it)
	{
		if (it->second != std::numeric_limits<int>::max())
		{
			reachable = true;
			break ;
		}
	}
	return !empty && reachable;
}

std::vector<Point>	get_neighbours(const Point& point, const Set& valid_points)
{
	std::vector<Point>	neighbours =
	{
		{point.x, point.y - 1},
		{point.x, point.y + 1},
		{point.x - 1, point.y},
		{point.x + 1, point.y}
	};

	for (auto it = neighbours.begin(); it != neighbours.end();)
	{
		Point&	neighbour = *it;
		if (valid_points.contains(neighbour) == false)
		{
			it = neighbours.erase(it);
		}
		else
		{
			++it;
		}
	}
	return neighbours;
}

std::deque<Point>	dijkstra(Map& map, Point& start, Point& end)
{
	Set		unvisited = construct_set(map);
	Paths	paths {};

	unvisited[start] = 0;
	while (should_check(unvisited))
	{
		auto	it = std::min_element(unvisited.begin(), unvisited.end(),
					 [](const std::pair<Point,int>& l, const std::pair<Point,int>& r)
					 { return l.second < r.second; });
		const Point&	pos = it->first;
		const int&		cost = it->second;
		if (pos == end)
		{
			break ;
		}
		std::vector<Point>	neighbours = get_neighbours(pos, unvisited);
		for (const auto& neighbour : neighbours)
		{
			int	new_cost = cost + 1;
			if (new_cost < unvisited[neighbour])
			{
				unvisited[neighbour] = new_cost;
				paths[neighbour] = pos;
			}
		}
		unvisited.erase(it);
	}
	Point&				prev = paths[end];
	std::deque<Point>	path_to_exit {};
	while (prev != start)
	{
		path_to_exit.push_front(prev);
		prev = paths[prev];
	}
	return path_to_exit;
}

std::set<Point>	find_walls_next_to_path(Map& map, std::deque<Point>& path)
{
	std::set<Point>	walls {};

	for (auto it = path.begin(); it != path.end(); ++it)
	{
		Point&	tile = *it;
		Point	up {tile.x, tile.y - 1};
		Point	down {tile.x, tile.y + 1};
		Point	left {tile.x - 1, tile.y};
		Point	right {tile.x + 1, tile.y};
		if (map[up.y][up.x] == '#')
			walls.insert(up);
		if (map[down.y][down.x] == '#')
			walls.insert(down);
		if (map[left.y][left.x] == '#')
			walls.insert(left);
		if (map[right.y][right.x] == '#')
			walls.insert(right);
	}
	return walls;
}

using Cheats = std::map<int, int>;

int	main()
{
	Map	map = parse("input.txt");
	Point	start = Point::find_in(map, 'S');
	Point	end = Point::find_in(map, 'E');

	print(map);
	std::deque<Point> path = dijkstra(map, start, end);
	std::set<Point>	walls_next_to_path = find_walls_next_to_path(map, path);

	Cheats	cheats {};
	for (auto rit = walls_next_to_path.rbegin(); rit != walls_next_to_path.rend(); ++rit)
	{
		Point	disabled_wall = *rit;
		Map		cheat_map {map};
		cheat_map[disabled_wall.y][disabled_wall.x] = '.';
		std::deque<Point>	cheat_path = dijkstra(cheat_map, start, end);
		int	picoseconds_saved = (path.size() + 1) - (cheat_path.size() + 1);
		if (picoseconds_saved > 0)
		{
			++cheats[picoseconds_saved];
		}
	}
	int	count {0};
	for (auto it = cheats.begin(); it != cheats.end(); ++it)
	{
		std::cout << "Time saved: " << it->first << ", " << it->second << " amount of times\n";
		if (it->first >= 100)
		{
			++count;
		}
	}
}