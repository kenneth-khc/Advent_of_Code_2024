#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <queue>
#include <vector>
#include <unordered_map>
#include <map>
#include "Point.hpp"

enum Direction
{
	NORTH = 0,
	SOUTH,
	EAST,
	WEST
};

struct Node
{
	Node() = default;
	Node(const Node&) = default;
	Node(Point<> point):
	coords(point),
	dir(EAST),
	cost(-1) {}
	Node(Point<> point, Direction dir, int cost):
	coords(point),
	dir(dir),
	cost(cost)
	{
		if (dir == NORTH)
			d = Point(0, -1);
		else if (dir == SOUTH)
			d = Point(0, +1);
		else if (dir == EAST)
			d = Point(+1, 0);
		else
			d = Point(-1, 0);
	}

	Point<>		coords;
	Direction	dir;
	Point<>		d;
	int			cost;

	bool	operator<(const Node& rhs) const
	{
		return this->coords < rhs.coords;
	}

	struct Compare
	{
		bool	operator()(const Node& n1, const Node& n2) const
		{
			return n1.cost > n2.cost;
		}
	};
};

using Map = std::vector<std::vector<char>>;
// using Set = std::unordered_map<Point<>,int,Point<>::Hasher>;
// using Paths = std::unordered_map<Point<>,std::vector<Point<>>,Point<>::Hasher>;
// using Prio_Queue = std::priority_queue<Node, std::vector<Node>, Node::Compare>;
using Path = std::vector<Node>;
using Paths = std::map<Node, std::vector<Path>>;

Paths	dijkstra(Map& map, Point<>& start_pos, Point<>& end_pos);


#endif