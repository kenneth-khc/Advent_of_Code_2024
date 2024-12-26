#include "dijkstra.hpp"

static Set
construct_unvisited_set(Map&);
static bool
should_check(Set&);
static Point<>
get_xy_change(Direction&);
static std::vector<Point<int>>
get_neighbours(const Point<int>&, const Set&);
static Direction
get_direction(const Point<int>&, const Point<int>&);
static void
update(const std::vector<Point<int>>&, Paths&, const Point<int>&,
	   const Point<int>&, const uint64_t&, Set&);

Paths	dijkstra(Map& map, Point<int>& start_pos, Point<int>& end_pos)
{
	(void)end_pos;
	Direction	reindeer_dir = Direction::EAST;
	Set			unvisited = construct_unvisited_set(map);
	Paths		paths {};

	unvisited[start_pos] = 0;
	while (should_check(unvisited))
	{
		auto	it = std::min_element(unvisited.begin(), unvisited.end(),
		[](const std::pair<Point<int>,uint64_t>& l, const std::pair<Point<int>,uint64_t>& r){
			return l.second < r.second;
		});	
		const Point<int>&	curr_pos = it->first;
		const uint64_t&	curr_cost = it->second;
		if (curr_pos == end_pos)
		{
			std::cout << "Cheapest cost to exit: " << curr_cost << std::endl;
			// break ;
		}
		if (unvisited.empty())
		{
			std::cout << "Every tile is visited!\n";
			break ;
		}

		if (paths.contains(curr_pos))
		{
			for (auto& predecessor : paths[curr_pos])
			{
				Point<int>&	prev = predecessor;
				reindeer_dir = get_direction(curr_pos, prev);
			}
		}
		const Point		change = get_xy_change(reindeer_dir);

		std::vector<Point<int>>	neighbours =
		get_neighbours(curr_pos, unvisited);
		update(neighbours, paths, change, curr_pos, curr_cost, unvisited);
		unvisited.erase(it);
	}
	return paths;
}

static Set
construct_unvisited_set(Map& map)
{
	Set	set;

	for (size_t y = 0; y < map.size(); ++y)
	{
		for (size_t x = 0; x < map[y].size(); ++x)
		{
			if (map[y][x] != '#')
			{
				Point<>		new_point (x, y);
				uint64_t	cost {std::numeric_limits<uint64_t>::max()};
				set.emplace(new_point, cost);
			}
		}
	}
	return set;
}

static bool	should_check(Set& unvisited)
{
	bool	empty = unvisited.empty();
	bool	reachable = false;
	for (auto it = unvisited.begin(); it != unvisited.end(); ++it)
	{
		if (it->second != std::numeric_limits<uint64_t>::max())
		{
			reachable = true;
			break ;
		}
	}
	return !empty && reachable;
}


static Point<>	get_xy_change(Direction& dir)
{
	if (dir == Direction::EAST) return {+1, 0};
	else if (dir == Direction::NORTH) return {0, -1};
	else if (dir == Direction::WEST) return {-1, 0};
	else return {0, +1};
}

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
