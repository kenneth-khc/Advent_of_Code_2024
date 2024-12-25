#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using Locks = std::vector<std::vector<std::vector<char>>>;
using Keys = std::vector<std::vector<std::vector<char>>>;

template<typename T>
void	add_to(T& container, std::vector<std::vector<char>> schematic)
{
	container.push_back(schematic);
}

void	parse(const std::string& filename, Locks& locks, Keys& keys)
{
	std::ifstream	file {filename};
	std::string		line {};
	while (std::getline(file, line))
	{
		std::vector<std::vector<char>>	buffer {};
		const std::string&	first_line = line.data();

		buffer.push_back(std::vector(line.begin(), line.end()));
		while (std::getline(file, line) && !line.empty())
		{
			buffer.push_back(std::vector(line.begin(), line.end()));
		}
		if (first_line[0] == '#')
		{
			add_to(locks, buffer);
		}
		else
		{
			add_to(keys, buffer);
		}
	}
}

std::vector<int>	count_height(std::vector<std::vector<char>>& schematic, int y, int y_end)
{
	std::vector<int>	heights {};
	int	dy = y < y_end ? +1 : -1;

	for (size_t x = 0; x < schematic[y].size(); ++x)
	{
		int	height {0};
		for (int curr_y = y; curr_y != y_end; curr_y += dy)
		{
			if (schematic[curr_y][x] == '#')
			{
				++height;
			}
			else
			{
				break ;
			}
		}
		heights.push_back(height);
	}
	return heights;
}

int	main()
{
	Locks	locks {};
	Keys	keys {};

	parse("input.txt", locks, keys);

	int	pairs_fit {0};
	for (auto it = locks.begin(); it != locks.end(); ++it)
	{
		std::vector<int>	lock_height = count_height(*it, 1, it->size()-1);
		for (auto it2 = keys.begin(); it2 != keys.end(); ++it2)
		{
			std::vector<int>	key_height = count_height(*it2, it2->size()-2, 0);
			std::vector<int>	sum {};
			std::transform(lock_height.begin(), lock_height.end(),
						   key_height.begin(), std::back_inserter(sum),
						   std::plus<int>());
			if (std::find_if(sum.begin(), sum.end(),
			   [&](const int& num)
			   { return num > 5; }) == sum.end())
			{
				++pairs_fit;
			}
		}
	}
	std::cout << pairs_fit << " key/lock pairs fit\n";
}