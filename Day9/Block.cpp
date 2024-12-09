#include "Block.hpp"

Block::Block(int count):
size(count)
{

}

void Block::resize(std::size_t new_size)
{
    size = new_size;
}