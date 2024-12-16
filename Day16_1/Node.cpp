#include <limits>
#include <unordered_set>
#include "Node.hpp"

Node::Node(const Point& p):
pos(p),
cost(std::numeric_limits<uint64_t>::max())
{

}

bool    Node::operator==(const Node& other) const
{
    return this->pos.x == other.pos.x && this->pos.y == other.pos.y;
}

bool    Node::operator==(const Point& other) const
{
    return this->pos.x == other.x && this->pos.y == other.y;
}
