#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <optional>
#include <set>
#include "Antenna.hpp"
#include "Antinode.hpp"

using std::size_t;

/* get the antinodes of 2 antennas with the same frequency by 
finding their distance apart */
std::optional<std::set<Antinode>>
get_antinodes(const Antenna& a1, const Antenna& a2,
std::vector<std::string>& map)
{
	std::set<Antinode>			valid_antinodes;
	std::pair<int, int>	distance = Antenna::get_distance(a1, a2);
	std::pair<int, int>	inversed
		= std::make_pair<int, int>(-distance.first, -distance.second);

	std::pair<size_t, size_t>	new_index;
	int	i = 1;
	new_index = a1.calculate_antinode(distance, i);
	while (new_index.second < map.size() && new_index.first < map[0].size())
	{
		++i;
		valid_antinodes.insert(Antinode(new_index.first, new_index.second));
		new_index = a1.calculate_antinode(distance, i);
	}
	i = 1;
	new_index = a2.calculate_antinode(inversed, i);
	while (new_index.second < map.size() && new_index.first < map[0].size())
	{
		i++;
		valid_antinodes.insert(Antinode(new_index.first, new_index.second));
		new_index = a2.calculate_antinode(inversed, i);
	}

	if (valid_antinodes.empty())
	{
		return {};
	}
	else
	{
		return std::optional(valid_antinodes);
	}
}

std::set<Antinode>
find_antinodes(std::vector<Antenna>& antennas, std::vector<std::string>& map)
{
	std::set<Antinode>	antinodes;

	for (const auto& a1 : antennas)
	{
		for (const auto& a2 : antennas)
		{
			if (&a1 == &a2)
				continue ;
			if (Antenna::same_frequency(a1, a2))
			{
				auto	res = get_antinodes(a1, a2, map);
				if (res)
				{
					for (auto& node : res.value())
					{
						antinodes.insert(node);
					}
				}
			}
		}
	}
	return antinodes;
}

int main()
{
    std::fstream				file{"input.txt"};
	std::vector<std::string>	map;
	std::vector<Antenna>		antennas;
	std::set<Antinode>			antinodes;

	for (int y = 0; !file.eof(); y++)
	{
		int	x = 0;
		std::string	line;
		std::getline(file, line);
		map.push_back(line);
		for (char& c : line)
		{
			if (std::isalnum(c))
			{
				antennas.push_back(Antenna(x, y, c));
			}
			x++;
		}
	}
	antinodes = find_antinodes(antennas, map);
	std::cout << "Number of antinodes: " << antinodes.size() << std::endl;
}