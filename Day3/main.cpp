#include <regex>
#include <fstream>
#include <iostream>

int main()
{
    std::ifstream   file{"input.txt"};
    std::regex      mul_pattern{"mul\\(([0-9]{1,3}),([0-9]{1,3})\\)"};
    int             sum{0};

    while (!file.eof())
    {
        std::string buffer;
        std::getline(file, buffer);

        auto    words_begin = std::sregex_iterator(buffer.begin(), buffer.end(), mul_pattern);
        auto    words_end = std::sregex_iterator();

        while (words_begin != words_end)
        {
            std::smatch mul_match = *words_begin;
            int lhs = std::stoi(mul_match[1]);
            int rhs = std::stoi(mul_match[2]);
            int product = lhs * rhs;
            std::cout << lhs << " * " << rhs << " = " << product << std::endl;
            sum += product;
            ++words_begin;
        }
    }
    std::cout << "Sum is " << sum << std::endl;
}