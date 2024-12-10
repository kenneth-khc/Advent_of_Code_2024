#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstddef>
#include <utility>

using std::size_t;

std::vector<std::string>
parse_input(std::string filename);

uint32_t
count_peaks(std::vector<std::string>& map, size_t x, size_t y);

uint32_t
ascend(std::vector<std::string>& map, size_t x, size_t y, char target);

/* For part 1 where each peak is counted once only for a trailhead 
no matter how many different paths can get there */
std::set<std::pair<int, int>>	visited_peaks;

int main()
{
	std::vector<std::string>	map = parse_input("input.txt");
	uint32_t	sum {0};

	for (size_t y = 0; y < map.size(); ++y)
	{
		for (size_t x = 0; x < map[y].size(); ++x)
		{
			uint32_t	score {0};

			if (map[y][x] == '0')
			{
				score = count_peaks(map, x, y);
				sum += score;
				visited_peaks.clear();
			}
		}
	}
	std::cout << "Sum of all trailheads is " << sum << std::endl;
}

std::vector<std::string>
parse_input(std::string filename)
{
    std::vector<std::string>	map;
    std::ifstream	file {filename};
	std::string		line;

	while (file >> line)
	{
		map.push_back(line);
	}
	return map;
}

uint32_t
count_peaks(std::vector<std::string>& map, size_t x, size_t y)
{
	uint32_t	count {0};

	count += ascend(map, x + 1, y, '1');
	count += ascend(map, x - 1, y, '1');
	count += ascend(map, x, y + 1, '1');
	count += ascend(map, x, y - 1, '1');

	return count;
	return visited_peaks.size(); // for part 1
}

uint32_t
ascend(std::vector<std::string>& map, size_t x, size_t y, char target)
{
	if (y >= map.size() || x >= map[y].size() || map[y][x] != target)
	{
		return false;
	}
	if (target == '9')
	{
		visited_peaks.insert(std::make_pair(x, y));
		return true;
	}

	return ascend(map, x + 1, y, target + 1)
		 + ascend(map, x - 1, y, target + 1)
		 + ascend(map, x, y + 1, target + 1)
		 + ascend(map, x, y - 1, target + 1);
}