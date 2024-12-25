#include "Box.hpp"
#include "Point.hpp"

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

bool	Box::operator==(const Box& rhs) const
{
	return this->left == rhs.left && this->right == rhs.right;
}