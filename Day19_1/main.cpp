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

bool	match(std::string design, Towels& available)
{
	// Done matching the entire design
	if (design.empty())
	{
		return true;
	}
	for (const std::string& towel : available)
	{
		if (design.starts_with(towel))
		{
			std::string	remain = design.substr(towel.length());
			if (match(remain, available))
			{
				return true;
			}
		}
	}
	return false;
}

int	calculate_possible_designs(Towels& available, Towels& designs)
{
	int	possible_designs {0};
	for (std::string design : designs)
	{
		print("Matching with " + design + '\n');
		if (match(design, available))
		{
			print("\e[0;32m" + design + " is possible!\n" + "\e[0;0m");
			++possible_designs;
		}
		else
		{
			print("\e[0;31m" + design + " is impossible!\n" + "\e[0;0m");
		}
	}
	return possible_designs;
}

int	main(int argc, char** argv)
{
	if (argc != 2)
		return 1;

	Towels	available_towels {};
	Towels	requirements = parse(argv[1], available_towels);

	std::cout << "Towels available: ";
	print(available_towels);

	int	possible_designs = calculate_possible_designs(available_towels, requirements);

	print("There are ");
	print(possible_designs);
	print(" possible designs\n");

	return 0;
}