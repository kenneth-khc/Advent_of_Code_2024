#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <limits>
#include <numeric>

std::map<uint64_t,uint64_t>
get_stones(const std::string filename);

bool
even_digits(const uint64_t& num);

std::pair<uint64_t,uint64_t>
split_stone(const uint64_t& num);

int	main()
{
	std::map<uint64_t,uint64_t>	stones = get_stones("input.txt");
	const int					max_blinks = 75;

	for (int blink_count = 1; blink_count <= max_blinks; ++blink_count)
	{
		std::map<uint64_t,uint64_t>	new_stones = stones;
		std::cout << "\nBlinking " << blink_count << " times\n";
		for (auto curr = stones.begin(); curr != stones.end(); ++curr)
		{
			uint64_t	num = curr->first;
			uint64_t	frequency = curr->second;

			if (frequency == 0) continue ;
			if (num == 0)
			{
				std::cout << frequency << " [" << num << "] -> " << frequency << " [1]\n";
				new_stones[1] += frequency;
				new_stones[num] -= frequency;
			}
			else if (even_digits(num))
			{
				std::pair<uint64_t,uint64_t>	split_stones = split_stone(num);
				std::cout << frequency << " [" << num << "] -> " << frequency
						  << " (" << split_stones.first << ")(" << split_stones.second << ")\n";
				new_stones[split_stones.first] += frequency;
				new_stones[split_stones.second] += frequency;
				new_stones[num] -= frequency;
			}
			else
			{
				uint64_t	multiplied_num = num * 2024;
				std::cout << frequency << " [" << num << "] -> " << frequency << " [" << multiplied_num << "]\n";
				new_stones[multiplied_num] += frequency;
				new_stones[num] -= frequency;
			}
		}
		stones = std::move(new_stones);
	}
	// oops, I have to pass in 0ULL or else it'll pass in 0 as an i32
	// and the accumulator overflows. wasted a lot of time on this...
	uint64_t	sum = std::accumulate(stones.begin(), stones.end(), 0ULL,
		[](uint64_t val, const std::map<uint64_t, uint64_t>::value_type& pair)
		{
			return val + pair.second;
		});
	std::cout << "Frequency of all stones is: " << sum << std::endl;
}

std::map<uint64_t,uint64_t>	get_stones(const std::string filename)
{
	std::map<uint64_t,uint64_t>	stones {};
	std::ifstream				file {filename};
	uint64_t					stone_num {0};

	while (file >> stone_num)
	{
		++stones[stone_num];
	}
	return stones;
}

bool	even_digits(const uint64_t& num)
{
	return std::to_string(num).length() % 2 == 0;
}

std::pair<uint64_t,uint64_t>	split_stone(const uint64_t& num)
{
	std::string	num_str = std::to_string(num);
	std::string	left_num_str = num_str.substr(0, num_str.size() / 2);
	std::string	right_num_str = num_str.substr(num_str.size() / 2, num_str.size());
	uint64_t	left_num = std::stoull(left_num_str);
	uint64_t	right_num = std::stoull(right_num_str);

	return std::make_pair(left_num, right_num);
}
