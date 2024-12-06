#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <vector>
#include <algorithm>

bool    is_safe(std::string& report)
{
	std::stringstream	ss{report};
	std::vector<int>	v;
	std::vector<int>	v_difference;

	std::string level;
	ss >> level;
	while (ss)
	{
		int	num = std::stoi(level);
		v.push_back(num);
		ss >> level;
	}
	auto	begin = v.begin();
	auto	it = ++begin;
	auto	end = v.end();
	std::adjacent_difference(begin, end, begin);
	bool	difference_ok = std::all_of(it, end, [](int x)
	{
		return abs(x) >= 1 && abs(x) <= 3;
	});
	bool	same_sign = std::all_of(begin, end, [](int x)
	{

	});
	return (false);
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


}