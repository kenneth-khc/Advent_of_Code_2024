#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include "Point.hpp"
#include "Node.hpp"

std::vector<std::vector<char>>
parse_map(const std::string& filename);

enum class Direction {
	EAST = 0,
	NORTH,
	WEST,
	SOUTH
};

Point	find_point(std::vector<std::vector<char>>& map, const char& c)
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
construct_set(std::vector<std::vector<char>>& map)
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

int main()
{
	std::vector<std::vector<char>>	map = parse_map("example.txt");
	Point		exit_pos = find_point(map, 'E');
	Point		reindeer_pos = find_point(map, 'S');
	Direction	reindeer_dir = Direction::EAST;
	Set			unvisited = construct_set(map);
	unvisited[reindeer_pos] = 0;

	while (should_check(unvisited))
	{
		auto	it = std::min_element(unvisited.begin(), unvisited.end(),
		[](const std::pair<Point,uint64_t>& l, const std::pair<Point,uint64_t>& r){
			return l.second < r.second;
		});

		const Point&	curr_pos = it->first;
		const uint64_t&	curr_cost = it->second;
		const Point		change = get_xy_change(reindeer_dir);
		const Point		forward_tile = {curr_pos.x + change.x, curr_pos.y + change.y};
		// update neighbours
		std::vector<std::pair<Point,Direction>>	neighbours =
		{
			{{curr_pos.x + 1, curr_pos.y}, Direction::EAST},
			{{curr_pos.x, curr_pos.y - 1}, Direction::NORTH},
			{{curr_pos.x - 1, curr_pos.y}, Direction::WEST},
			{{curr_pos.x, curr_pos.y + 1}, Direction::SOUTH}
		};
		for (const auto& neighbour : neighbours)
		{
			if (unvisited.contains(neighbour.first) == false) continue;
			uint64_t	new_cost = curr_cost;
			if (neighbour.second != reindeer_dir)
			{
				new_cost += 1000;
				reindeer_dir = neighbour.second;
			}
			new_cost += 1;
			if (new_cost < unvisited[neighbour.first])
			{
				unvisited[neighbour.first] = new_cost;
			}
		}
		if (curr_pos == exit_pos)
		{
			std::cout << "Cost to exit: " << curr_cost << std::endl;
		}
		unvisited.erase(it);
	}
	return 0;
}

std::vector<std::vector<char>>
parse_map(const std::string& filename)
{
    std::ifstream	file {filename};
    std::vector<std::vector<char>>	map;
	std::string	line;

	while (file >> line)
	{
		map.push_back(std::vector(line.begin(), line.end()));
	}
	return map;
}
