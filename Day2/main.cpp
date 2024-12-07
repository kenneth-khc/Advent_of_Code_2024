#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <vector>
#include <algorithm>

std::vector<int>	vectorize(std::string& report)
{
	std::stringstream	ss{report};
	std::vector<int>	v;

	std::string level;
	ss >> level;
	while (ss)
	{
		int	num = std::stoi(level);
		v.push_back(num);
		ss >> level;
	}
	return (v);
}

bool    is_safe(std::string& report)
{
	std::vector<int>	v = vectorize(report);

	auto	begin = v.begin();
	auto	it = begin + 1;
	auto	end = v.end();
	int		corrected{0};

	std::adjacent_difference(begin, end, begin);

	for (auto left = it, right = it + 1; left != end; ++left, ++right)
	{
		int&	lhs = *left;
		int&	rhs = *right;
		
		if (abs(lhs) == 0 || abs(lhs) > 3)
		{
			++corrected;
		}
		if (!((lhs < 0) == (rhs < 0)))
		{
			++corrected;
		}
	}
	
	if (corrected <= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main()
{
    std::ifstream	file{"input.txt"};
    int				safe_count{0};

    while (!file.eof())
    {
        std::string	report;
        std::getline(file, report);

        if (is_safe(report))
        {
			safe_count++;
        }
    }
	std::cout << "Safe count: " << safe_count << std::endl;
}