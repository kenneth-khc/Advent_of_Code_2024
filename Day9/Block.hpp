#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstddef>

class Block
{
public:
    std::size_t size;

public:
    Block(/* args */) = default;
    Block(int);
    virtual ~Block() = default;
    virtual void    visualize() const = 0;
    void    resize(std::size_t);
};

#endif