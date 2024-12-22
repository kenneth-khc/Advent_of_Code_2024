#include <fstream>
#include <iostream>
#include <vector>
#include <string>

std::vector<uint64_t>	parse(const std::string& filename)
{
	std::vector<uint64_t>	numbers {};

	std::ifstream	file {filename};
	std::string		line {};
	while (file >> line)
	{
		numbers.push_back(std::stoull(line));
	}
	return numbers;
}

int	main()
{
	std::vector<uint64_t>	numbers = parse("input.txt");
	
	uint64_t	sum_2000th_nums {0};
	for (uint64_t& number : numbers)
	{
		for (int count = 0; count < 2000; ++count)
		{
			uint64_t	multiply = number * 64;
			number ^= multiply;
			number %= 16777216;

			uint64_t	divide = number / 32;
			number ^= divide;
			number %= 16777216;

			uint64_t	mult2048 = number * 2048;
			number ^= mult2048;
			number %= 16777216;
		}
		std::cout << "> " << number << '\n';
		sum_2000th_nums += number;
	}
	std::cout << "Sum of all 2000th numbers is " << sum_2000th_nums << '\n';
}