#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <chrono>
#include <thread>
#include "Point.hpp"
#include "Robot.hpp"
#include "Map.hpp"

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

int	main()
{
	Map	map(101, 103);
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
		map = copy;
	}
	int	quad_1 {0};
	for (int y = 0; y < map.height / 2; ++y)
	{
		for (int x = map.width / 2 + 1; x < map.width; ++x)
		{
			quad_1 += map.layout[y][x];
		}
	}
	int	quad_2 {0};
	for (int y = 0; y < map.height / 2; ++y)
	{
		for (int x = 0; x < map.width / 2; ++x)
		{
			quad_2 += map.layout[y][x];
		}
	}
	int	quad_3 {0};
	for (int y = map.height / 2 + 1; y < map.height; ++y)
	{
		for (int x = 0; x < map.width / 2; ++x)
		{
			quad_3 += map.layout[y][x];
		}
	}
	int	quad_4 {0};
	for (int y = map.height / 2 + 1; y < map.height; ++y)
	{
		for (int x = map.width / 2 + 1; x < map.width; ++x)
		{
			quad_4 += map.layout[y][x];
		}
	}
	std::cout << "Q1: " << quad_1 << '\n';
	std::cout << "Q2: " << quad_2 << '\n';
	std::cout << "Q3: " << quad_3 << '\n';
	std::cout << "Q4: " << quad_4 << '\n';
	std::cout << "Safety factor: " << quad_1 * quad_2 * quad_3 * quad_4 << std::endl;
	return 0;
}
