#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include "Block.hpp"

class File: public Block
{
private:

public:
    File(/* args */) = default;
    ~File() = default;
    File(int, std::size_t);

    void    visualize() const override;
    std::string get_type() const override;
};

#endif