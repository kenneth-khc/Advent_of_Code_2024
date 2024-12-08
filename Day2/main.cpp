#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <vector>
#include <algorithm>

static bool
is_safe(std::string& report);

static std::vector<int>
vectorize(std::string& report);

static bool
dampen_problem(std::vector<int>& report);

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

static bool
is_safe(std::string& report)
{
	std::vector<int>	v = vectorize(report);
	std::vector<int>	diff(v.size());

	std::adjacent_difference(v.begin(), v.end(), diff.begin());
	diff.erase(diff.begin());

	bool	all_positive = std::all_of(diff.begin(), diff.end(), [](int x){return x > 0;});
	bool	all_negative = std::all_of(diff.begin(), diff.end(), [](int x){return x < 0;});
	bool	steps_ok = std::all_of(diff.begin(), diff.end(), [](int x){return abs(x) > 0 && abs(x) < 4;});

	if (steps_ok && (all_positive || all_negative))
	{
		return true;
	}
	else
	{
		return dampen_problem(v);
	}
}

static std::vector<int>
vectorize(std::string& report)
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

static bool
dampen_problem(std::vector<int>& report)
{
	for (size_t i = 0; i < report.size(); ++i)
	{
		std::vector<int>	test{report};
		test.erase(test.begin() + i);
		std::adjacent_difference(test.begin(), test.end(), test.begin());
		test.erase(test.begin());
		bool	all_positive = std::all_of(test.begin(), test.end(), [](int x){return x > 0;});
		bool	all_negative = std::all_of(test.begin(), test.end(), [](int x){return x < 0;});
		bool	steps_ok = std::all_of(test.begin(), test.end(), [](int x){return abs(x) > 0 && abs(x) < 4;});
		if (steps_ok && (all_positive || all_negative))
		{
			return true;
		}
	}
	return false;
}
