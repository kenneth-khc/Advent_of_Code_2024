#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

using Towels = std::vector<std::string>;

Towels
parse(const std::string& filename, Towels& available)
{
	std::ifstream	file {filename};
	Towels			requirements {};
	std::string		line {};
	std::regex		pattern {R"(\w+)"};

	std::getline(file, line);
	auto	curr = std::sregex_iterator(line.begin(), line.end(), pattern);
	auto	end = std::sregex_iterator {};
	while (curr != end)
	{
		std::smatch		match = *curr;
		available.push_back(match.str());
		++curr;
	}
	while (file >> line)
	{
		requirements.push_back(line);
	}
	return requirements;
}

bool	match(const char* word, const std::string& letters)
{
	size_t	i = 0;
	while (letters[i] == word[i])
	{
		i++;
	}
	return i >= letters.length();
}

bool	match_towels(std::string requirement, Towels& available)
{
	size_t	i = 0;
	for (auto& towel : available)
	{
		if (match(&requirement[i], towel))
		{
			i += towel.length();
		}
	}
	return i >= requirement.length();
}

int	main()
{
	Towels	available_towels {};
	Towels	requirements = parse("example.txt", available_towels);

	int		possible_designs {0};
	for (auto& requirement : requirements)
	{
		if (match_towels(requirement, available_towels))
			++possible_designs;
	}
	std::cout << "There are " << possible_designs << " possible designs" << std::endl;
}