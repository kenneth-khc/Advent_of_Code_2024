#include <iostream>
#include "Block.hpp"
#include "FreeSpace.hpp"

FreeSpace::FreeSpace(int size):
Block(size)
{

}

void    FreeSpace::visualize() const
{
    for (size_t i = 0; i < size; ++i)
    {
        std::cout << '.';
    }
}