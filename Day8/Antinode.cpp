#include "Antinode.hpp"

Antinode::Antinode(int x, int y):
x(x),
y(y)
{

}

bool    Antinode::operator==(const Antinode& rhs) const
{
    return this->x == rhs.x && this->y == rhs.y;
}