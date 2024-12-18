#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include "Point.hpp"

using Map = std::vector<std::vector<char>>;

Map
parse_map(const std::string&);

Point
find_point(Map&, const char&);

void
dijkstra(Map&, Point&, Point&);

int main()
{
	Map			map = parse_map("input.txt");
	Point		exit_pos = find_point(map, 'E');
	Point		reindeer_pos = find_point(map, 'S');

	dijkstra(map, reindeer_pos, exit_pos);
	return 0;
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

Point	find_point(Map& map, const char& c)
{
	for (size_t y = 0; y < map.size(); ++y)
	{
		for (size_t x = 0; x < map[y].size(); ++x)
		{
			if (map[y][x] == c)
			{
				return Point(x, y);
			}
		}
	}
	return Point(-1, -1);
}

using Set = std::unordered_map<Point,uint64_t,Point::Hasher>;

Set
construct_set(Map&);

bool
should_check(Set&);

enum class Direction {
	EAST = 0,
	NORTH,
	WEST,
	SOUTH
};

Direction
get_direction(const Point&, const Point&);

Point
get_xy_change(Direction& dir);

std::vector<Point>
get_neighbours(const Point&, const Set&);

using Paths = std::unordered_map<Point,Point,Point::Hasher>;

void
update(const std::vector<Point>&, Paths&, const Point&,
	   const Point&, const uint64_t&, Set&);

void	dijkstra(Map& map, Point& start_pos, Point& end_pos)
{
	Direction	reindeer_dir = Direction::EAST;
	Set			unvisited = construct_set(map);
	Paths		paths {};

	unvisited[start_pos] = 0;
	while (should_check(unvisited))
	{
		auto	it = std::min_element(unvisited.begin(), unvisited.end(),
		[](const std::pair<Point,uint64_t>& l, const std::pair<Point,uint64_t>& r){
			return l.second < r.second;
		});	
		const Point&	curr_pos = it->first;
		const uint64_t&	curr_cost = it->second;
		if (curr_pos == end_pos)
		{
			std::cout << "Cost to exit: " << curr_cost << std::endl;
			break ;
		}

		if (paths.contains(curr_pos))
		{
			Point&	prev = paths[curr_pos];
			reindeer_dir = get_direction(curr_pos, prev);
		}
		const Point		change = get_xy_change(reindeer_dir);

		std::vector<Point>	neighbours =
		get_neighbours(curr_pos, unvisited);
		update(neighbours, paths, change, curr_pos, curr_cost, unvisited);
		unvisited.erase(it);
	}

}

Set
construct_set(Map& map)
{
	Set	set;

	for (size_t y = 0; y < map.size(); ++y)
	{
		for (size_t x = 0; x < map[y].size(); ++x)
		{
			if (map[y][x] != '#')
			{
				set.insert({Point(x, y),
							std::numeric_limits<uint64_t>::max()});
			}
		}
	}
	return set;
}

bool	should_check(Set& unvisited)
{
	bool	empty = unvisited.empty();
	bool	reachable = false;
	for (auto it = unvisited.begin(); it != unvisited.end(); ++it)
	{
		if (it->second != std::numeric_limits<uint64_t>::max())
		{
			reachable = true;
			break ;
		}
	}
	return !empty && reachable;
}

Point	get_xy_change(Direction& dir)
{
	if (dir == Direction::EAST) return {+1, 0};
	else if (dir == Direction::NORTH) return {0, -1};
	else if (dir == Direction::WEST) return {-1, 0};
	else return {0, +1};
}

Direction	get_direction(const Point& curr, const Point& prev)
{
	Point	diff = curr - prev;

	if (diff == Point {1, 0}) return Direction::EAST;
	else if (diff == Point {-1, 0}) return Direction::WEST;
	else if (diff == Point {0, -1}) return Direction::NORTH;
	else return Direction::SOUTH;
}

std::vector<Point>
get_neighbours(const Point& point, const Set& valid_points)
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

void	update(const std::vector<Point>& neighbours, Paths& paths, const Point& change,
			   const Point& curr_pos, const uint64_t& curr_cost, Set& unvisited)
{
	for (const auto& neighbour : neighbours)
	{
		uint64_t	new_cost = curr_cost + 1;
		if (neighbour - curr_pos != change)
		{
			new_cost += 1000;
		}
		if (new_cost < unvisited[neighbour])
		{
			unvisited[neighbour] = new_cost;
			paths[neighbour] = curr_pos;
		}
	}
}
