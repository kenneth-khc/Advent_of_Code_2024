#include <iostream>
#include "File.hpp"

File::File(int id, int size):
Block(size),
id(id)
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