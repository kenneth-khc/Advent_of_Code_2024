#ifndef FILE_HPP
#define FILE_HPP

#include "Block.hpp"

class File: public Block
{
private:

public:
    int id;
    File(/* args */) = default;
    File(int, int);
    ~File() = default;
    void	visualize() const override;
};

#endif