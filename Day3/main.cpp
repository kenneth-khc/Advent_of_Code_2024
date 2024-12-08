#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>

std::string	file_to_str(std::string filename)
{
    std::ifstream   	file {filename};
	std::stringstream	buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

int main()
{
	std::string	str = file_to_str("input.txt");
    std::regex	pattern {"(do\\(\\))|(don't\\(\\))|(mul\\((\\d+),(\\d+)\\))"};
	bool		do_operation {true};
	int			sum {0};

	auto	current = std::sregex_iterator(str.begin(), str.end(), pattern);
	auto	end = std::sregex_iterator();
	while (current != end)
	{
		std::smatch	match = *current;
		std::cout << match.str() << std::endl;
		if (match.str() == "do()")
		{
			do_operation = true;
		}
		else if (match.str() == "don't()")
		{
			do_operation = false;
		}
		else if (do_operation)
		{
			int	lhs = std::stoi(match[4].str());	
			int	rhs = std::stoi(match[5].str());	
			int	product = lhs * rhs;
			std::cout << lhs << " * " << rhs << " = " << product << std::endl;
			sum += product;
		}
		++current;
	}
	std::cout << "Sum of all products is " << sum << std::endl;
}

// int main() {
//     std::ifstream   file{"example.txt"};
//     std::regex      mul_pattern{"mul\\(([0-9]{1,3}),([0-9]{1,3})\\)"};
//     int             sum{0};

//     while (!file.eof())
//     {
//         std::string buffer;
//         std::getline(file, buffer);

//         auto    words_begin = std::sregex_iterator(buffer.begin(), buffer.end(), mul_pattern);
//         auto    words_end = std::sregex_iterator();

//         while (words_begin != words_end)
//         {
//             std::smatch mul_match = *words_begin;
//             int lhs = std::stoi(mul_match[1]);
//             int rhs = std::stoi(mul_match[2]);
//             int product = lhs * rhs;
//             std::cout << lhs << " * " << rhs << " = " << product << std::endl;
//             sum += product;
//             ++words_begin;
//         }
//     }
//     std::cout << "Sum is " << sum << std::endl;
// }
