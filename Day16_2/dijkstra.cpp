#include <queue>
#include <map>
#include <utility>
#include "dijkstra.hpp"

using Map = std::vector<std::vector<char>>;
// using Set = std::map<Node,int,Node::Compare>;
using DistanceMap = std::unordered_map<Point<>,int,Point<>::Hasher>;

static DistanceMap
construct_distance_map(Map&);
#if 0
static bool
should_check(Set&);
static std::vector<Point<int>>
get_neighbours(const Point<int>&, const Set&);
static Point<>
get_xy_change(Direction&);
static Direction
get_direction(const Point<int>&, const Point<int>&);
static void
update(const std::vector<Point<int>>&, Paths&, const Point<int>&,
	   const Point<int>&, const uint64_t&, Set&);
static bool	should_check(Set& unvisited)
{
	bool	empty = unvisited.empty();
	bool	reachable = false;
	for (auto it = unvisited.begin(); it != unvisited.end(); ++it)
	{
		if (it->second != std::numeric_limits<int>::max())
		{
			reachable = true;
			break ;
		}
	}
	return !empty && reachable;
}


#endif

using Prio_Queue = std::priority_queue<Node, std::vector<Node>, Node::Compare>;
using Path = std::vector<Node>;
using Paths = std::map<Node, std::vector<Path>>;

Node	get_next_node(Node& curr)
{
	Point	next_point = {curr.coords.x + curr.d.x,
						  curr.coords.y + curr.d.y};
	return Node(next_point, curr.dir, 1); // cost is 1 for moving forward
}

std::pair<Node,Node>	get_turn_nodes(Node& curr)
{
	Point	first, second;
	if (curr.dir == NORTH || curr.dir == SOUTH)
	{
		first = {curr.coords.x-1, curr.coords.y};
		second = {curr.coords.x+1, curr.coords.y};
		return std::make_pair(Node(first, WEST, 1001),
							  Node(second, EAST, 1001));
	}
	else if (curr.dir == EAST || curr.dir == WEST)
	{
		first = {curr.coords.x, curr.coords.y-1};
		second = {curr.coords.x, curr.coords.y+1};
		return std::make_pair(Node(first, NORTH, 1001),
							  Node(second, SOUTH, 1001));
	}
	return {};
}

Paths	dijkstra(Map& map, Point<int>& start_pos, Point<int>& end_pos)
{
	DistanceMap	distances = construct_distance_map(map);
	distances[start_pos] = 0;
	Prio_Queue	pq {};
	Paths		paths {};
	paths[Node(start_pos)] = {{start_pos}};
	pq.push(Node(start_pos, EAST, 0));
	while (!pq.empty())
	{
		Node	visiting = pq.top();
		if (visiting.coords == end_pos)
		{
			std::cout << "Found!\n";
			std::cout << "Total cost: " << visiting.cost << '\n';
		}
		pq.pop();
		// auto it = distances.find(visiting.coords);
		// if (it != distances.end())
		// 	distances.erase(it);

		std::vector<Node>		neighbours {};
		std::pair<Node,Node>	turns = get_turn_nodes(visiting);
		neighbours.push_back(get_next_node(visiting));
		neighbours.push_back(turns.first);
		neighbours.push_back(turns.second);
		for (auto it = neighbours.begin(); it != neighbours.end();)
		{
			if (map[it->coords.y][it->coords.x] == '#')
				neighbours.erase(it);
			// if (!distances.contains(it->coords))
			// 	neighbours.erase(it);
			else
				++it;
		}
		for (auto& neighbour : neighbours)
		{
			int	new_cost = visiting.cost + neighbour.cost;
			// paths[neighbour].push_back(visiting);
			if (new_cost < distances[neighbour.coords])
			{
				distances[neighbour.coords] = new_cost;
				// paths[neighbour.coords] = {};
				for (std::vector<Node> path : paths[neighbour])
				{
					std::vector<Node>	new_path = path;
					new_path.push_back(visiting);
					paths[visiting].push_back(new_path);
				}
				neighbour.cost = new_cost;
				pq.push(neighbour);
				// paths[neighbour].push_back(visiting);
			}
			else if (new_cost == distances[neighbour.coords])
			{
				for (std::vector<Node> path : paths[visiting])
				{
					std::vector<Node> new_path = path;
					new_path.push_back(neighbour.coords);
					paths[neighbour].push_back(new_path);
				}
			}
		}
	}
	return paths;
}

static DistanceMap
construct_distance_map(Map& map)
{
	DistanceMap	set {};

	for (size_t y = 0; y < map.size(); ++y)
	{
		for (size_t x = 0; x < map[y].size(); ++x)
		{
			if (map[y][x] != '#')
			{
				Point<>		new_point (x, y);
				uint64_t	cost {std::numeric_limits<int>::max()};
				set.emplace(new_point, cost);
			}
		}
	}
	return set;
}

#if 0
static std::vector<Point<>>
get_neighbours(const Point<>& point, const Set& valid_points)
{
	std::vector<Point<>>	neighbours =
	{
		{point.x, point.y - 1},
		{point.x, point.y + 1},
		{point.x - 1, point.y},
		{point.x + 1, point.y}
	};

	for (auto it = neighbours.begin(); it != neighbours.end();)
	{
		Point<>&	neighbour = *it;
		if (valid_points.contains(neighbour) == false)
		{
			it = neighbours.erase(it);
		}
		else
		{
			++it;
		}
	}
	return neighbours;
}

static Point<>	get_xy_change(Direction& dir)
{
	if (dir == Direction::EAST) return {+1, 0};
	else if (dir == Direction::NORTH) return {0, -1};
	else if (dir == Direction::WEST) return {-1, 0};
	else return {0, +1};
}

static void	update(const std::vector<Point<>>& neighbours, Paths& paths, const Point<>& change,
			   const Point<>& curr_pos, const uint64_t& curr_cost, Set& unvisited)
{
	for (const auto& neighbour : neighbours)
	{
		uint64_t	new_cost = curr_cost + 1;
		if (neighbour - curr_pos != change)
		{
			new_cost += 1000;
		}
		if (new_cost <= unvisited[neighbour])
		{
			unvisited[neighbour] = new_cost;
			paths[neighbour].clear();
			paths[neighbour].push_back(curr_pos);
		}
		else if (new_cost == unvisited[neighbour])
			paths[neighbour].push_back(curr_pos);
	}
}

static Direction	get_direction(const Point<>& curr, const Point<>& prev)
{
	Point<>	diff = curr - prev;

	if (diff == Point {1, 0}) return Direction::EAST;
	else if (diff == Point {-1, 0}) return Direction::WEST;
	else if (diff == Point {0, -1}) return Direction::NORTH;
	else return Direction::SOUTH;
}

#endif