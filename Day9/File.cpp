#include <iostream>
#include "File.hpp"

File::File(int id, std::size_t size):
Block(id, size)
{

}

void    File::visualize() const
{
    std::cout << "|";
    for (size_t i = 0; i < size; ++i)
    {
        std::cout << id;
        if (i != size - 1)
        {
            std::cout << ",";
        }
    }
    std::cout << "|";
}

std::string File::get_type() const
{
    return "File";
}