#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <optional>
#include "Antenna.hpp"
#include "Antinode.hpp"

/* get the antinode of 2 antennas with the same frequency by 
finding their distance apart */
std::optional<Antinode>	get_antinode(const Antenna& a1, const Antenna& a2,
std::vector<std::string>& map)
{
	std::pair<int, int>	distance = Antenna::get_distance(a1, a2);
	std::pair<std::size_t, std::size_t> node_pos
	= a2.calculate_antinode(distance);

	if (node_pos.second >= map.size() ||
		node_pos.first >= map[0].size())
	{
		return {};
	}
	return Antinode(node_pos.first, node_pos.second);
}

std::vector<Antinode>
find_antinodes(std::vector<Antenna>& antennas, std::vector<std::string>& map)
{
	std::vector<Antinode>	antinodes;

	for (const auto& a1 : antennas)
	{
		for (const auto& a2 : antennas)
		{
			if (&a1 == &a2)
				continue ;
			if (Antenna::same_frequency(a1, a2))
			{
				auto node = get_antinode(a1, a2, map);
				if (node)
				{
					if (std::find(antinodes.begin(), antinodes.end(), node.value()) == antinodes.end())
					{
						antinodes.push_back(node.value());					
					}
				}
			}
		}
	}
	return antinodes;
}

int main()
{
    std::fstream				file{"example.txt"};
	std::vector<std::string>	map;
	std::vector<Antenna>		antennas;
	std::vector<Antinode>		antinodes;

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