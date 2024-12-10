#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstddef>
#include <string>

class Block
{
public:
    int         id;
    std::size_t size;

    Block(/* args */) = default;
    Block(int, std::size_t);
    virtual ~Block() = default;

    virtual void visualize() const = 0;
    virtual std::string get_type() const = 0;
    void resize(std::size_t);
};

#endif