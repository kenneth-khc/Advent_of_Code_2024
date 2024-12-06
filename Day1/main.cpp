#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <sstream>

void
process_file(std::multiset<int>& list_1, std::multiset<int>& list_2)
{
    std::fstream	file{"input.txt"}; 
    std::string		buffer;

    while (!file.eof())
    {
        std::getline(file, buffer);
		std::stringstream	line{buffer};
		std::string	str;
		line >> str;
		int	num = std::stoi(str);
		list_1.insert(num);
		line >> str;
		num = std::stoi(str);
		list_2.insert(num);
    }
}

void
find_differences(std::multiset<int> list_1, std::multiset<int> list_2)
{
	int	sum{0};

	while (!list_1.empty())
	{
		auto	l1_smallest{list_1.begin()};
		auto	l2_smallest{list_2.begin()};
		int		difference = std::abs(*l1_smallest - *l2_smallest);
		list_1.erase(l1_smallest);
		list_2.erase(l2_smallest);
		sum += difference;
	}
	std::cout << "Sum of differences is: " << sum << std::endl;
}

void
find_similarity(std::multiset<int> list_1, std::multiset<int> list_2)
{
	int	sum{0};

	while (!list_1.empty())
	{
		auto	l1_smallest{list_1.begin()};
		int		count = list_2.count(*l1_smallest);
		sum += (*l1_smallest * count);
		list_1.erase(l1_smallest);
	}
	std::cout << "Sum of similarity score is: " << sum << std::endl;
}

int main()
{
	std::multiset<int>	list_1;
	std::multiset<int>	list_2;

	process_file(list_1, list_2);
	find_differences(list_1, list_2);
	find_similarity(list_1, list_2);
}