#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include "Point.hpp"
#include "Robot.hpp"
#include "Map.hpp"

std::vector<Robot>
parse_input(const std::string& filename);

int
count_robots_in_quadrant(Map& map, Point start, Point end);

int	main()
{
	Map	map {101, 103};
	std::vector<Robot>	robots = parse_input("input.txt");
	for (int seconds = 0; seconds < 100; ++seconds)
	{
		Map	copy {101, 103};
		for (auto& robot : robots)
		{
			robot.move(copy);
			copy.increment_grid(robot.pos);
		}
		map = copy;
	}
	int	quad_1 = count_robots_in_quadrant(map, Point(map.width/2+1, 0), Point(map.width, map.height/2));
	int	quad_2 = count_robots_in_quadrant(map, Point(0, 0), Point(map.width/2, map.height/2));
	int	quad_3 = count_robots_in_quadrant(map, Point(0, map.height/2+1), Point(map.width/2, map.height));
	int	quad_4 = count_robots_in_quadrant(map, Point(map.width/2+1, map.height/2+1), Point(map.width, map.height));
	std::cout << "Q1: " << quad_1 << '\n';
	std::cout << "Q2: " << quad_2 << '\n';
	std::cout << "Q3: " << quad_3 << '\n';
	std::cout << "Q4: " << quad_4 << '\n';
	std::cout << "Safety factor: " << quad_1 * quad_2 * quad_3 * quad_4 << std::endl;
	return 0;
}

std::vector<Robot>	parse_input(const std::string& filename)
{
	std::vector<Robot>	robots;
	std::ifstream		file {filename};
	std::string			line;
	std::smatch			match;
	std::regex			pattern {R"(p=(\d+),(\d+) v=([-\d]+),([-\d]+))"};

	std::getline(file, line);
	while (file)
	{
		std::regex_match(line, match, pattern);
		Point	pos {std::stoi(match[1]), std::stoi(match[2])};
		Point	velo {std::stoi(match[3]), std::stoi(match[4])};
		Robot	new_robot = Robot(pos, velo);
		robots.push_back(new_robot);
		std::getline(file, line);
	}
	return robots;
}

int	count_robots_in_quadrant(Map& map, Point start, Point end)
{
	int	count {0};
	int	y = start.y;
	while (y != end.y)
	{
		int	x = start.x;
		while (x != end.x)
		{
			count += map.layout[y][x];
			++x;
		}
		++y;
	}
	return count;
}