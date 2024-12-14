#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include "Point.hpp"
#include "Robot.hpp"
#include "Map.hpp"

std::vector<Robot>
parse_input(const std::string& filename);

bool
robots_all_spread_out(Map& map);

int	main()
{
	Map	map {101, 103};
	std::vector<Robot>	robots = parse_input("input.txt");
	for (int iteration = 1; iteration < 10000; ++iteration)
	{
		std::cout << "Iteration " << iteration << '\n';
		std::ofstream output {"iterations/iter" + std::to_string(iteration)};
		Map	copy {101, 103};
		for (auto& robot : robots)
		{
			robot.move(copy);
			copy.increment_grid(robot.pos);
		}
		copy.visualize(output);
		if (robots_all_spread_out(copy))
		{
			std::cout << "Each tile has less than 1 robot on iteration " << iteration << '\n';
			break ;
		}
		map = copy;
	}
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

bool	robots_all_spread_out(Map& map)
{
	for (int y = 0; y < map.height; ++y)
	{
		for (int x = 0; x < map.width; ++x)
		{
			if (map.layout[y][x] > 1)
			{
				return false;
			}
		}
	}
	return true;
}