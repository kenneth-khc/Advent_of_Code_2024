#ifndef FREESPACE_HPP
#define FREESPACE_HPP

#include "Block.hpp"

class FreeSpace: public Block
{
private:
    /* data */
public:
    FreeSpace(/* args */) = default;
    FreeSpace(int);
    ~FreeSpace() = default;
    void    visualize() const override;
};

#endif