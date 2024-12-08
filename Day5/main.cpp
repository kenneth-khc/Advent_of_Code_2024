#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <utility>
#include <regex>
#include <algorithm>

using RuleMap = std::unordered_map<int, std::vector<int>>;

RuleMap
parse_rules(std::fstream& file);

std::vector<int>
get_update(std::fstream& file);

int	main()
{
	std::fstream		file {"input.txt"};
	RuleMap				rules = parse_rules(file);
	std::vector<int>	update;
	int					valid_sum {0};
	int					invalid_sum {0};
	// int	count = 0;

	while (!(update = get_update(file)).empty())
	{
		bool	fixed = false;
		for (size_t curr = 0; curr < update.size(); ++curr)
		{
			const int	current_num = update[curr];
			const std::vector<int>&	current_rule = rules[current_num];
			for (size_t prev = 0; prev < curr; ++prev)
			{
				const int&	current_check = update[prev]; (void)current_check;
				if (std::find(current_rule.begin(), current_rule.end(), current_check) != current_rule.end())
				{
					fixed = true;
					update.erase(update.begin() + curr);
					update.insert(std::next(update.begin(), prev), current_num);
				}
			}
		}
		if (fixed)
		{
			invalid_sum += *(update.begin() + update.size() / 2);
		}
		else
		{
			valid_sum += *(update.begin() + update.size() / 2);
		}
	}
	std::cout << "Sum of the midpoint of valid numbers: " << valid_sum << '\n'
			  << "Sum of the midpoint of invalid numbers: " << invalid_sum << std::endl;
}

RuleMap
parse_rules(std::fstream& file)
{
	std::string	line;
	RuleMap		rules;
	std::regex	rule_pattern {"((\\d+)\\|(\\d+))"};

	while (file)
	{
		std::getline(file, line);
		if (line.empty())
			break ;
		std::smatch	match;
		std::regex_match(line, match, rule_pattern);
		int	key = std::stoi(match[2]);
		int	value = std::stoi(match[3]);
		rules[key].push_back(value);
	}
	return rules;
}

std::vector<int>
get_update(std::fstream& file)
{
	std::string			line;
	std::regex			number_pattern {"(\\d+)"};
	std::vector<int>	update;
	if (file >> line)
	{
		auto current = std::sregex_iterator(line.begin(), line.end(), number_pattern);
		auto end = std::sregex_iterator();
		while (current != end)
		{
			std::smatch	match = *current;
			update.push_back(std::stoi(match.str()));
			++current;
		}
	}
	return update;
}
