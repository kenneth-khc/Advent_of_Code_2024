#include <iostream>
#include "Block.hpp"
#include "FreeSpace.hpp"

FreeSpace::FreeSpace(std::size_t size):
Block(-1, size)
{

}

void    FreeSpace::visualize() const
{
    for (size_t i = 0; i < size; ++i)
    {
        std::cout << '.';
    }
}

std::string FreeSpace::get_type() const
{
    return "FreeSpace";
}