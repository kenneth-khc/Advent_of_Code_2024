#include <cstddef>
#include <unordered_map>
#include "Point.hpp"

Point::Point(int x, int y):
x(x),
y(y)
{

}

bool    Point::operator==(const Point& other) const
{
    return this->x == other.x && this->y == other.y;
}

Point   Point::operator-(const Point& other) const
{
    int new_x = this->x - other.x;
    int new_y = this->y - other.y;
    return Point(new_x, new_y);
}

size_t  Point::Hasher::operator()(const Point& p) const
{
    size_t  x_hash = std::hash<int>()(p.x);
    size_t  y_hash = std::hash<int>()(p.y) << 1;
    return x_hash ^ y_hash;
};