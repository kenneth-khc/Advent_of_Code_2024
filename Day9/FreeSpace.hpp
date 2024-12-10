#ifndef FREESPACE_HPP
#define FREESPACE_HPP

#include <string>
#include "Block.hpp"

class FreeSpace: public Block
{
private:
    /* data */
public:
    FreeSpace(/* args */) = default;
    FreeSpace(std::size_t);
    ~FreeSpace() = default;
    void    visualize() const override;
    std::string get_type() const override;
};

#endif