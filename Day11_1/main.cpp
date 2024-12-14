#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<uint64_t>
get_stones(const std::string filename);

bool
even_digits(const uint64_t& num);

std::pair<uint64_t,uint64_t>
split_stones(const uint64_t& num);

int	main()
{
	std::vector<uint64_t>	stones = get_stones("input.txt");
	const int				max_blinks = 25;

	for (int blink_count = 1; blink_count <= max_blinks; ++blink_count)
	{
		std::cout << "Blinking " << blink_count << (blink_count == 1 ? " time\n" : " times\n");
		for (auto curr = stones.begin(); curr != stones.end(); ++curr)
		{
			if (*curr == 0)
			{
				*curr = 1;
			}
			else if (even_digits(*curr))
			{
				std::pair<int,int> new_stones = split_stones(*curr);
				*curr = new_stones.second;
				curr = stones.insert(curr, new_stones.first) + 1;
			}
			else
			{
				*curr *= 2024;
			}
		}
	}
	std::cout << "Final " << stones.size() << " stones: ";
	for (const auto& stone : stones)
	{
		std::cout << stone << " ";
	}
	std::cout << '\n' << stones.size() << " stones." << std::endl;
}

std::vector<uint64_t>	get_stones(const std::string filename)
{
	std::ifstream			file {filename};
	std::vector<uint64_t>	stones {};
	uint64_t				stone_num {0};

	while (file >> stone_num)
	{
		stones.push_back(stone_num);
	}
	return stones;
}

bool	even_digits(const uint64_t& num)
{
	return std::to_string(num).length() % 2 == 0;
}

std::pair<uint64_t,uint64_t>	split_stones(const uint64_t& num)
{
	std::string	num_str = std::to_string(num);
	std::string	left_num_str = num_str.substr(0, num_str.size() / 2);
	std::string	right_num_str = num_str.substr(num_str.size() / 2, num_str.size());
	uint64_t	left_num = std::stoi(left_num_str);
	uint64_t	right_num = std::stoi(right_num_str);

	return std::make_pair(left_num, right_num);
}
