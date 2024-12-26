#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <map>

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
		std::smatch	match = *curr;
		available.push_back(match.str());
		++curr;
	}
	while (file >> line)
	{
		requirements.push_back(line);
	}
	return requirements;
}

template<typename T>
void	print(T t)
{
	std::cout << t;
}

void	print(const std::vector<std::string>& strings)
{
	for (auto it = strings.begin(); it != strings.end(); ++it)
	{
		print(*it);
		if (it != strings.end() - 1)
		{
			print(" | ");
		}
	}
	print('\n');
}

std::map<std::string,bool>	memo {};

#include <algorithm>
bool	match(std::string design,
			  Towels& available,
			  std::vector<std::string>& combination,
			  std::vector<std::vector<std::string>>& all_combinations)
{
	bool	matched = false;
	// Done matching the entire design
	if (design.empty())
	{
		all_combinations.push_back(combination);
		auto it = std::accumulate(combination.begin(), combination.end(),)
		matched = true;
		return true;
	}
	if (memo.find(design) != memo.end())
	{
		return memo[design];
	}
	for (const std::string& towel : available)
	{
		if (design.starts_with(towel))
		{
			combination.push_back(towel);
			std::string	remain = design.substr(towel.length());
			if (match(remain, available, combination, all_combinations))
				matched = true;
			combination.pop_back();
		}
	}
	memo[design] = matched;
	return matched;
}

void	find_total_combinations(Towels& available, Towels& designs)
{
	std::vector<std::vector<std::string>>	total_combinations {};
	for (std::string design : designs)
	{
		std::vector<std::string>	combinations {};
		print("Matching with " + design + '\n');
		match(design, available, combinations, total_combinations);
	}
	print("Total combinations: ");
	print(total_combinations.size());
	print("\n");
}

int	main(int argc, char** argv)
{
	if (argc != 2)
		return 1;

	Towels	available_towels {};
	Towels	requirements = parse(argv[1], available_towels);

	std::cout << "Towels available: ";
	print(available_towels);

	find_total_combinations(available_towels, requirements);

	// print("There are ");
	// print(possible_designs);
	// print(" possible designs\n");

	return 0;
}