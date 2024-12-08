#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <cstdint>

using std::uint64_t;

uint64_t	concat_numbers(uint64_t n1, uint64_t n2)
{
	std::string joined = std::to_string(n1) + std::to_string(n2);
	return std::stoul(joined);
}

bool	check_validity(uint64_t target, uint64_t result, std::queue<uint64_t> operands)
{
	if (operands.empty() && result == target)
	{
		return true;
	}
	else if (operands.empty() && result != target)
	{
		return false;
	}
	int	num = operands.front();
	operands.pop();
	if (result == 0)
	{
		if (check_validity(target, num, operands))
			return true;
	}
	else
	{
		if (check_validity(target, result * num, operands) ||
			check_validity(target, result + num, operands) ||
			check_validity(target, concat_numbers(result, num), operands))
		{
			return true;
		}
	}
	return false;
}

int main()
{
	std::fstream	file{"input.txt"};
	std::regex		pattern{"([\\d]+)"};
	uint64_t		sum{0};

	while (!file.eof())
	{
		std::string	line;
		std::getline(file, line);
		auto begin = std::sregex_iterator(line.begin(), line.end(), pattern);
		auto end = std::sregex_iterator();
		uint64_t	target = std::stol(begin->str());

		std::queue<uint64_t>	operands;
		for (auto curr = ++begin; curr != end; ++curr)
		{
			operands.push(std::stol(curr->str()));
		}
		uint64_t	result{0};
		if (check_validity(target, result, operands))
			sum += target;
	}
	std::cout << "Sum of valid operations is " << sum << std::endl;
}