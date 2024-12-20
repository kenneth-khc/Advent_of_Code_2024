#include <iostream>
#include <vector>
#include <array>
#include <set>
#include <fstream>
#include <string>
#include "Point.hpp"
#include <utility>

using Map = std::vector<std::vector<char>>;
using std::size_t;

Map	parse(const std::string& filename)
{
	std::ifstream	file {filename};
	std::string		line {};
	Map				map {};
	while (file >> line)
	{
		map.push_back(std::vector(line.begin(), line.end()));
	}
	return map;
}

void	print(const Map& map)
{
	for (size_t row = 0; row < map.size(); ++row)
	{
		for (size_t column = 0; column < map[row].size(); ++column)
		{
			std::cout << map[row][column];
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

bool	in_map(Map& map, Point& pos)
{
	size_t	x = pos.x;
	size_t	y = pos.y;
	return y < map.size() && x < map[y].size();
}

char&	ele_at(const Point& pos, Map& map)
{
	return map[pos.y][pos.x];
}

void	turn(int& direction)
{
	direction = (direction + 1) % 4;
}

void	move(Point& point, int& direction, Map& map)
{
	static const std::array<const Point, 4>	change =
	{
		Point(0, -1),
		Point(+1, 0),
		Point(0, +1),
		Point(-1, 0),
	};
	Point	next_point = point + change[direction];
	while (in_map(map, next_point) && ele_at(next_point, map) == '#')
	{
		turn(direction);
		next_point = point + change[direction];
	}
	// if (in_map(map, point))
	// 	ele_at(point, map) = '.';
	point = next_point;
}

enum Direction
{
	NORTH = 0,
	EAST,
	SOUTH,
	WEST
};

std::set<Point>	patrol(Map map, Point guard_pos)
{
	std::set<Point>	path {};
	int				dir = Direction::NORTH;
	while (in_map(map, guard_pos))
	{
		path.insert(guard_pos);
		move(guard_pos, dir, map);
	}
	std::cout << "Number of unique tiles visited: " << path.size() << '\n';
	return path;
}

bool	infinite_loop(Map map, const Point& obstruction, Point tortoise)
{
	Point	hare = tortoise;
	int		hare_dir = Direction::NORTH;
	int		tortoise_dir = Direction::NORTH;
	ele_at(obstruction, map) = '#';
	while (in_map(map, hare))
	{
		move(tortoise, tortoise_dir, map);
		move(hare, hare_dir, map);
		move(hare, hare_dir, map);
		// if (in_map(map, hare))
		// 	ele_at(hare, map) = 'H';
		// if (in_map(map, tortoise))
		// 	ele_at(tortoise, map) = 'T';
		// print(map);
		if (tortoise == hare && tortoise_dir == hare_dir)
		{
			return true;
		}
	}
	return false;
}

int main()
{
	Map				map = parse("input.txt");
	Point			guard = Point::find_in(map, '^');
	std::set<Point>	tiles_visited = patrol(map, guard);

	int	possible_obstructions {0};
	for (auto it = tiles_visited.begin(); it != tiles_visited.end(); ++it)
	{
		const Point	&tile = *it;
		if (infinite_loop(map, tile, guard))
		{
			++possible_obstructions;
		}
	}
	std::cout << "Number of possible obstructions: " << possible_obstructions << '\n';
}