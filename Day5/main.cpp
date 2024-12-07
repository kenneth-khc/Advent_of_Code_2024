#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <regex>
#include <algorithm>

std::map<int, std::vector<int>>
get_page_rules(std::fstream& file)
{
	std::map<int, std::vector<int>>	page_rules;
	std::regex	rule_pattern{"([\\d]+)\\|([\\d]+)"};

	std::string	buffer;
	std::getline(file, buffer);
	while (!buffer.empty())
	{
		std::smatch	match;
		std::regex_search(buffer, match, rule_pattern);
		int	num1 = std::stoi(match[1]);
		int	num2 = std::stoi(match[2]);
		if (page_rules.find(num1) == page_rules.end())
		{
			page_rules.insert(std::make_pair(num1, std::vector<int>()));
		}
		page_rules[num1].push_back(num2);
		std::getline(file, buffer);
	}
	return page_rules;
}

using	match_it = std::regex_iterator<std::string::const_iterator>;

std::vector<int>	vectorize_production(std::string& line)
{
	std::vector<int>	production;
	std::regex			number_pattern{"[\\d]+"};

	match_it	begin = std::sregex_iterator(line.begin(),
											 line.end(),
											 number_pattern);
	match_it	end = std::sregex_iterator();
	for (match_it current = begin; current != end; ++current)
	{
		int	num = std::stoi(current->str());
		production.push_back(num);
	}
	return production;
}

// void
// fix_invalid
// (
// std::vector<int> prod,
// std::map<int, std::vector<int>> page_rules,
// int& invalid_sum
// )
// {
// 	std::vector<int>::iterator	rules_begin = page_rules.begin();
// 	std::vector<int>::iterator	rules_begin = page_rules.begin();
// }

int main()
{
    std::fstream					file{"input.txt"};
	std::map<int, std::vector<int>>	page_rules = get_page_rules(file);

	int	valid_sum{0};
	int	fixed_invalid_sum{0};
	for (std::string buffer; getline(file, buffer);)
	{
		std::vector<int>					prod = vectorize_production(buffer);
		bool								valid = true;
		std::vector<int>::iterator			begin = prod.begin();
		std::vector<int>::reverse_iterator	rbegin = prod.rbegin();
		std::vector<int>::reverse_iterator	rend = prod.rend();
		while (rbegin != rend)
		{
			std::cout << *rbegin << std::endl;
			int	key = *rbegin;
			std::vector<int>::iterator	rules_begin = page_rules[key].begin();
			std::vector<int>::iterator	rules_end = page_rules[key].end();
			for (std::vector<int>::iterator curr{begin}; curr != rbegin.base(); ++curr)
			{
				key = *rbegin;
				auto found = std::find(rules_begin, rules_end, *curr);
				if (found != rules_end)
				{
					valid = false;
					auto prod_to_fix = std::find(begin, prod.end(), *found);
					prod.insert(prod_to_fix, key);
					prod.erase(rbegin.base());
					rbegin = prod.rbegin();
				}
			}
			++rbegin;
		}
		auto	mid = prod.begin() + prod.size() / 2;
		if (valid)
		{
			valid_sum += *mid;
		}
		else
		{
			fixed_invalid_sum += *mid;
		}
	}
	std::cout << "Valid sum is " << valid_sum << std::endl;
	std::cout << "Fixed invalid sum is " << fixed_invalid_sum << std::endl;
	return 0;
}