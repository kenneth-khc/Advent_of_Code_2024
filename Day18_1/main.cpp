#include <fstream>
#include <iostream>
#include <deque>
#include <vector>
#include <unordered_map>
#include <regex>
#include <limits>
#include "Point.hpp"

using Map = std::vector<std::vector<char>>;
using Set = std::unordered_map<Point,int,Point::Hasher>;

void
parse_file(const std::string&, Map&);

void
print(Map&);

Set
construct_set(Map&);

void
dijkstra(Set&);

const int   MAP_HEIGHT = 71;
const int   MAP_WIDTH = 71;

int main()
{
    Map map {MAP_HEIGHT, std::vector<char>(MAP_WIDTH, '.')};

	parse_file("input.txt", map);
    // print(map);

	Set	unvisited = construct_set(map);
	dijkstra(unvisited);
}

void	parse_file(const std::string& filename, Map& map)
{
	std::ifstream	file {filename};
	std::string		line;
	std::regex		pattern {R"((\d+),(\d+))"};
	std::smatch		match;

	size_t	bytes = 0;
	while (file >> line && bytes < 1024)
	{
		std::regex_match(line, match, pattern);
		int	x = std::stoi(match[1]);
		int	y = std::stoi(match[2]);
		map[y][x] = '#';
		++bytes;
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
							std::numeric_limits<int>::max()});
			}
		}
	}
	return set;
}

bool
should_check(Set&);

std::vector<Point>
get_neighbours(const Point&, const Set&);

void	dijkstra(Set& unvisited)
{
	Point	start_pos {0, 0};
	unvisited[start_pos] = 0;
	Point	end_pos {70, 70};

	while (should_check(unvisited))
	{
		auto	it = std::min_element(unvisited.begin(), unvisited.end(),
		[](const std::pair<Point,int>& l, const std::pair<Point,int>&r)
		{
			return l.second < r.second;
		});

	const Point&	curr_pos = it->first;
	const int&		curr_cost = it->second;

	std::vector<Point>	neighbours = get_neighbours(curr_pos, unvisited);
	for (const auto& neighbour : neighbours)
	{
		int	new_cost = curr_cost + 1;

		if (new_cost < unvisited[neighbour])
		{
			unvisited[neighbour] = new_cost;
		}
	}
	if (curr_pos == end_pos)
	{
		std::cout << "Cost to exit: " << curr_cost << std::endl;
	}
	unvisited.erase(it);
	}
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

void    print(Map& map)
{
    for (size_t y = 0; y < map.size(); ++y)
    {
        for (size_t x = 0; x < map[y].size(); ++x)
        {
            std::cout << map[y][x] << ' ';
        }
        std::cout << '\n';
    }
}