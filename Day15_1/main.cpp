#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include "Point.hpp"

std::vector<std::vector<char>>
parse_map(std::ifstream& file, Point& robot_pos);

std::deque<char>
parse_movement(std::ifstream& file);

bool
move_box(Point& box_pos,
		 std::vector<std::vector<char>>& map,
		 Point& diff);

void
move_robot(Point& robot_pos,
		   std::vector<std::vector<char>>& map, 
		   Point& diff);

void
print_map(std::vector<std::vector<char>>& map);

uint64_t
calculate_gps(std::vector<std::vector<char>>& map);

int	main()
{
	std::ifstream					file {"input.txt"};
	Point							robot_pos;
	std::vector<std::vector<char>>	map = parse_map(file, robot_pos);
	std::deque<char>				movement = parse_movement(file);

	while (!movement.empty())
	{
		Point	d {0, 0};
		char&	move = movement.front();

		if (move == '^')
		{
			d.x = 0; d.y = -1;
			std::cout << "Moving up\n";
		}
		else if (move == '<')
		{
			d.x = -1; d.y = 0;
			std::cout << "Moving left\n";
		}
		else if (move == 'v')
		{
			d.x = 0; d.y = +1;
			std::cout << "Moving down\n";
		}
		else
		{
			d.x = +1; d.y = 0;
			std::cout << "Moving right\n";
		}
		move_robot(robot_pos, map, d);
		movement.pop_front();
		print_map(map);
	}

	uint64_t	gps = calculate_gps(map);
	std::cout << "Total gps is " << gps << std::endl;
}

std::vector<std::vector<char>>	parse_map(std::ifstream& file, Point& robot_pos)
{
	std::vector<std::vector<char>>	map;
	std::string						line;

	std::getline(file, line);
	while (!line.empty())
	{
		map.push_back(std::vector(line.begin(), line.end()));
		std::getline(file, line);
	}
	for (size_t y = 0; y < map.size(); ++y)
	{
		for (size_t x = 0; x < map[y].size(); ++x)
		{
			if (map[y][x] == '@')
			{
				robot_pos.x = x;
				robot_pos.y = y;
				break ;
			}
		}
	}
	return map;
}

std::deque<char>	parse_movement(std::ifstream& file)
{
	std::deque<char>	movement;
	std::string			line;

	while (file >> line)
	{
		std::deque<char>	temp {line.begin(), line.end()};
		movement.insert(movement.end(), temp.begin(), temp.end());
	}
	return movement;
}

bool	move_box(Point& box_pos,
				 std::vector<std::vector<char>>& map,
				 Point& diff)
{
	Point	pos {box_pos.x + diff.x, box_pos.y + diff.y};

	if (map[pos.y][pos.x] == '#')
	{
		return false;
	}
	else if (map[pos.y][pos.x] == '.')
	{
		map[box_pos.y][box_pos.x] = '.';
		map[pos.y][pos.x] = 'O';
		return true;
	}
	else
	{
		if (move_box(pos, map, diff))
		{
			map[box_pos.y][box_pos.x] = '.';
			map[pos.y][pos.x] = 'O';
			return true;
		}
		return false;
	}
}

void	move_robot(Point& robot_pos,
				   std::vector<std::vector<char>>& map, 
				   Point& diff)
{
	Point	pos {robot_pos.x + diff.x, robot_pos.y + diff.y};

	if (map[pos.y][pos.x] == '#')
	{
		return ;
	}
	else if (map[pos.y][pos.x] == '.')
	{
		map[robot_pos.y][robot_pos.x] = '.';
		map[pos.y][pos.x] = '@';
		robot_pos = pos;
	}
	else
	{
		if (move_box(pos, map, diff))
		{
			map[robot_pos.y][robot_pos.x] = '.';
			map[pos.y][pos.x] = '@';
			robot_pos = pos;
		}
	}

}

uint64_t	calculate_gps(std::vector<std::vector<char>>& map)
{
	uint64_t	total_gps {0};

	for (size_t y = 0; y < map.size(); ++y)
	{
		for (size_t x = 0; x < map[y].size(); ++x)
		{
			if (map[y][x] != 'O') continue;

			uint64_t	box_gps = (100 * y) + x;
			total_gps += box_gps;
		}
	}
	return total_gps;
}

void	print_map(std::vector<std::vector<char>>& map)
{
	for (const auto& row : map)
	{
		for (const char& cell : row)
		{
			std::cout << cell << ' ';
		}
		std::cout << '\n';
	}
}
