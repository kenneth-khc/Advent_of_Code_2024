#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <numeric>

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

void	generate_secret_numbers(std::vector<uint64_t>& numbers,
								std::vector<std::vector<int>>& all_prices,
								std::vector<std::vector<int>>& all_sequences)
{
	for (uint64_t& number : numbers)
	{
		std::vector<int>	prices {};
		for (int count = 0; count < 2000; ++count)
		{
			int	price = number % 10;
			prices.push_back(price);

			uint64_t	mult64 = number * 64;
			number ^= mult64;
			number %= 16777216;

			uint64_t	div32 = number / 32;
			number ^= div32;
			number %= 16777216;

			uint64_t	mult2048 = number * 2048;
			number ^= mult2048;
			number %= 16777216;
		}
		std::vector<int>	sequence {};
		std::adjacent_difference(prices.begin(), prices.end(),
								 std::back_inserter(sequence));
		sequence.erase(sequence.begin());
		prices.erase(prices.begin());
		all_sequences.push_back(sequence);
		all_prices.push_back(prices);
	}
}

using std::size_t;

struct VectorHasher
{
	size_t	operator()(const std::vector<int>& vec) const
	{
		size_t	hash = 0;
		for (int num : vec)
		{
			hash ^= std::hash<int>{}(num) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		}
		return hash;
	}
};

using Sequence = std::vector<int>;
using Price_Table = std::unordered_map<Sequence,uint64_t,VectorHasher>;

std::vector<int>	get_sequence(const std::vector<int>& seq,
											 size_t seq_end)
{
	std::vector<int>	sequence {};	
	size_t				seq_start {seq_end - 3};
	while (seq_start <= seq_end)
	{
		sequence.push_back(seq[seq_start]);
		++seq_start;
	}
	return sequence;
}

int	main()
{
	std::vector<uint64_t>				numbers = parse("input.txt");
	std::vector<std::vector<int>>		all_prices {};
	std::vector<std::vector<int>>		all_sequences {};
	Price_Table							price_table {};
	
	generate_secret_numbers(numbers, all_prices, all_sequences);

	for (size_t number = 0; number < all_prices.size(); ++number)
	{
		const std::vector<int>&	prices = all_prices[number];
		const std::vector<int>&	sequences = all_sequences[number];
		std::unordered_set<Sequence,VectorHasher>	curr_num_sequences {};
		
		// start at 3 because valid sequences have to be of size 4
		for (size_t i = 3; i < sequences.size(); ++i)
		{
			std::vector<int>	sequence = get_sequence(sequences, i);
			// make sure to not add repeating sequences within the same number
			if (curr_num_sequences.contains(sequence)) continue;
			curr_num_sequences.insert(sequence);
			price_table[sequence] += prices[i];
		}
	}

	using pair_type = decltype(price_table)::value_type;
	auto it = std::max_element(
		std::begin(price_table), std::end(price_table),
		[] (const pair_type& p1, const pair_type& p2)
		{
			return p1.second < p2.second;
		});
	std::cout << "Using the sequence " 
			  << it->first[0] << " " << it->first[1] << " "
			  << it->first[2] << " " << it->first[3] << "\n";
	std::cout << "I can get " << it->second << " amount of bananas!\n";
}