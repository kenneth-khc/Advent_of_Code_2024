#ifndef NODE_HPP
#define NODE_HPP

#include <cstdint>
#include "Point.hpp"

using std::uint64_t;
using std::size_t;

class Node
{
public:
    Node() = default;
	Node(const Point&);
    ~Node() = default;

	bool	operator==(const Node&) const;
	bool	operator==(const Point&) const;

    Point		pos;
	uint64_t	cost;
};


#endif