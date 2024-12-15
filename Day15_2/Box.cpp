#include "Box.hpp"

Box::Box(int x, int y):
left(x, y),
right(x + 1, y)
{
}

Box::Box(const Point& left, const Point& right):
left(left),
right(right)
{
}
