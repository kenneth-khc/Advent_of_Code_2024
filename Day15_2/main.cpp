#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <utility>
#include <thread>
#include <chrono>
#include "Point.hpp"
#include "Box.hpp"

std::vector<std::vector<char>>
parse_map(std::ifstream& file, Point& robot_pos);

std::deque<char>
parse_movement(std::ifstream& file);

bool
move_box(Box& box,
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

void	print(std::deque<char> movements)
{
	// static int	moves = movements.size();
	static int	counter = 0;
	// static int	steps = moves / 10;

	std::cout << movements[0] << " | ";
	for (int i = 1; i < 10; ++i)
	{
		std::cout << movements[i] << "  ";
	}
	std::cout << '\t' << "Movement " << ++counter << '\n';
}

int	main()
{
	std::ifstream					file {"input.txt"};
	Point							robot_pos;
	std::vector<std::vector<char>>	map = parse_map(file, robot_pos);
	std::deque<char>				movement = parse_movement(file);

	std::cout << "Initial map\n";
	print_map(map);
	while (!movement.empty())
	{
		Point	d {0, 0};
		char&	move = movement.front();

		if (move == '^')
		{
			d.x = 0; d.y = -1;
			std::cout << "Moving up\t";
		}
		else if (move == '<')
		{
			d.x = -1; d.y = 0;
			std::cout << "Moving left\t";
		}
		else if (move == 'v')
		{
			d.x = 0; d.y = +1;
			std::cout << "Moving down\t";
		}
		else
		{
			d.x = +1; d.y = 0;
			std::cout << "Moving right\t";
		}
		print(movement);
		move_robot(robot_pos, map, d);
		movement.pop_front();
		print_map(map);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
	std::cout << "Done map\n";
	print_map(map);
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
		std::vector<char>	modified_map_row {};
		for (const char& c : line)
		{
			if (c == '#')
			{
				modified_map_row.push_back('#');
				modified_map_row.push_back('#');
			}
			else if (c == 'O')
			{
				modified_map_row.push_back('[');
				modified_map_row.push_back(']');
			}
			else if (c == '.')
			{
				modified_map_row.push_back('.');
				modified_map_row.push_back('.');
			}
			else if (c == '@')
			{
				modified_map_row.push_back('@');
				modified_map_row.push_back('.');
			}
		}
		map.push_back(modified_map_row);
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

bool	move_box(Box& box,
				 std::vector<std::vector<char>>& map,
				 Point& diff)
{
	if (diff.x != 0)
	{
		Point	new_pos;
		Box		moved_box;
		if (diff.x < 0)
		{
			new_pos = Point {box.left.x - 1, box.left.y};
			moved_box = Box {new_pos.x, new_pos.y};
		}
		else
		{
			new_pos = Point {box.right.x + 1, box.right.y};
			moved_box = Box {box.right.x, box.right.y};
		}
		const char&	c = map[new_pos.y][new_pos.x];
		if (c == '#')
		{
			return false;
		}
		else if (c == '.')
		{
			map[box.left.y][box.left.x] = '.';
			map[box.right.y][box.right.x] = '.';
			map[moved_box.left.y][moved_box.left.x] = '[';
			map[moved_box.right.y][moved_box.right.x] = ']';
			return true;
		}
		else
		{
			Box	next_box;
			if (diff.x < 0)
				next_box = {new_pos.x - 1, new_pos.y};
			else
				next_box = {new_pos.x, new_pos.y};
			if (move_box(next_box, map, diff))
			{
				map[box.left.y][box.left.x] = '.';
				map[box.right.y][box.right.x] = '.';
				map[moved_box.left.y][moved_box.left.x] = '[';
				map[moved_box.right.y][moved_box.right.x] = ']';
				return true;
			}
			return false;
		}
	}
	else
	{
		Point	new_p1;
		Point	new_p2;
		Box		moved_box;
		if (diff.y < 0)
		{
			new_p1 = Point{box.left.x, box.left.y - 1};
			new_p2 = Point{box.right.x, box.right.y - 1};
		}
		// else if (diff.y > 0)
		else
		{
			new_p1 = Point{box.left.x, box.left.y + 1};
			new_p2 = Point{box.right.x, box.right.y + 1};
		}
		moved_box = Box {new_p1.x, new_p1.y};
		const char&	c1 = map[new_p1.y][new_p1.x];
		const char&	c2 = map[new_p2.y][new_p2.x];
		if (c1 == '#' || c2 == '#')
		{
			return false;
		}
		else if (c1 == '.' && c2 == '.')
		{
			map[box.left.y][box.left.x] = '.';
			map[box.right.y][box.right.x] = '.';
			map[moved_box.left.y][moved_box.left.x] = '[';
			map[moved_box.right.y][moved_box.right.x] = ']';
			return true;
		}
		else
		{
			Box	next_box;
			if (c1 == ']' && c2 == '[')
			{
				next_box = Box {new_p1.x - 1, new_p1.y};
				Box	next_box_2 {new_p2.x, new_p2.y};
				if (move_box(next_box, map, diff)
				 && move_box(next_box_2, map, diff))
				{
					// Box	moved_box {new_p1.x, new_p1.y};
					map[box.left.y][box.left.x] = '.';
					map[box.right.y][box.right.x] = '.';
					map[moved_box.left.y][moved_box.left.x] = '[';
					map[moved_box.right.y][moved_box.right.x] = ']';
					return true;
				}
				return false;
			}
			else
			{
				if (c1 == ']')
				{
					next_box = Box {new_p1.x - 1, new_p1.y};
				}
				else if (c2 == '[')
				{
					next_box = Box {new_p2.x, new_p2.y};
				}
				else
				{
					next_box = Box {new_p1.x, new_p1.y};
				}
				if (move_box(next_box, map, diff))
				{
					// Box	moved_box {new_p1.x, new_p1.y};
					map[box.left.y][box.left.x] = '.';
					map[box.right.y][box.right.x] = '.';
					map[moved_box.left.y][moved_box.left.x] = '[';
					map[moved_box.right.y][moved_box.right.x] = ']';
					return true;
				}
				return false;
			}
		}
	}
}

void	move_robot(Point& robot_pos,
				   std::vector<std::vector<char>>& map, 
				   Point& diff)
{
	Point		pos {robot_pos.x + diff.x, robot_pos.y + diff.y};
	const char&	c = map[pos.y][pos.x];

	if (c == '#')
	{
		return ;
	}
	else if (c == '.')
	{
		map[robot_pos.y][robot_pos.x] = '.';
		map[pos.y][pos.x] = '@';
		robot_pos = pos;
	}
	else
	{
		Box	box;
		
		if (c == '[')
		{
			box = Box(pos.x, pos.y);
		}
		else
		{
			box = Box(pos.x - 1, pos.y);
		}
		if (move_box(box, map, diff))
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
			if (map[y][x] != '[') continue;

			uint64_t	box_gps = (100 * y) + x;
			total_gps += box_gps;
		}
	}
	return total_gps;
}
#include <cstdlib>

void	print_map(std::vector<std::vector<char>>& map)
{
	for (const auto& row : map)
	{
		for (const char& cell : row)
		{
			if (cell == '@')
			{
				std::cout << "ඞ";
			}
			else if (cell == '[')
			{
				std::cout << "\e[0;32m" << '[' << "\e[0m";
			}
			else if (cell == ']')
			{
				std::cout << "\e[0;32m" << ']' << "\e[0m";
			}
			else if (cell == '#')
			{
				std::cout << "\e[0;31m" << '#' << "\e[0m";
			}
			else
			{
				std::cout << cell;
			}
		}
		std::cout << '\n';
	}
	// for (size_t i = 0; i < map.size(); ++i)
	// {
	// 	std::cout << "\033[A";
	// }
}
