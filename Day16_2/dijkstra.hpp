#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Point.hpp"

using Map = std::vector<std::vector<char>>;
using Set = std::unordered_map<Point<>,uint64_t,Point<>::Hasher>;
using Paths = std::unordered_map<Point<>,std::vector<Point<>>,Point<>::Hasher>;

enum class Direction {
	EAST = 0,
	NORTH,
	WEST,
	SOUTH
};

Paths	dijkstra(Map& map, Point<>& start_pos, Point<>& end_pos);


#endif