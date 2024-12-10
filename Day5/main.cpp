#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <regex>
#include <algorithm>

bool
is_sorted(std::vector<int>&);

bool
compare(const int&, const int&);

// map a number to an array of all the numbers that it's lesser than
using RuleMap = std::unordered_map<int, std::vector<int>>;
RuleMap
parse_rules(std::fstream&);

std::vector<int>
get_update(std::fstream&);

RuleMap rules;

int	main()
{
	std::fstream		file {"input.txt"};
	std::vector<int>	update;
	int					valid_sum {0};
	int					invalid_sum {0};

	rules = parse_rules(file);
	while (!(update = get_update(file)).empty())
	{
        bool    sorted = is_sorted(update);
        if (sorted)
        {
		 	valid_sum += *(update.begin() + update.size() / 2);
        }
        else
        {
            // wtf I can just call std::sort and save all my troubles?
            std::sort(update.begin(), update.end(), compare);
            // crazy
		 	invalid_sum += *(update.begin() + update.size() / 2);
        }
	}
	std::cout << "Sum of the midpoint of valid updates: " << valid_sum << '\n'
			  << "Sum of the midpoint of invalid but fixed updates: " << invalid_sum << std::endl;
}

bool    is_sorted(std::vector<int>& update)
{
    for (size_t i = 0; i < update.size() - 1; ++i)
    {
        if (compare(update[i], update[i + 1]) == false)
            return false;
    }
    return true;
}

bool
compare(const int& a, const int& b)
{
    std::vector<int>&   greater = rules[a];
    auto lesser_than = std::find(greater.begin(), greater.end(), b);
    if (lesser_than != greater.end())
    {
        return true;
    }
    return false;
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