#include "Block.hpp"

Block::Block(int id, std::size_t size):
id(id),
size(size)
{

}

void Block::resize(std::size_t new_size)
{
    size = new_size;
}