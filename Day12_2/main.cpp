#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <deque>
#include <numeric>
#include <cctype>
#include <utility>
#include "Point.hpp"
#include "Plot.hpp"

using Map = std::vector<std::vector<char>>;

Map	parse(const std::string& filename)
{
	Map	map {};

	std::ifstream	file {filename};
	std::string		line;
	while (file >> line)
	{
		map.push_back(std::vector(line.begin(), line.end()));
	}
	return map;
}

void	print(std::vector<Point>& region)
{
	for (auto& plant : region)
	{
		std::cout << plant << '\n';
	}
}

void	print(Map& map)
{
	for (size_t row = 0; row < map.size(); ++row)
	{
		for (size_t column = 0; column < map[row].size(); ++column)
		{
			std::cout << map[row][column];
		}
		std::cout << '\n';
	}
}

bool	in_map(Map& map, const Point& p)
{
	size_t	x = p.x;
	size_t	y = p.y;
	return y < map.size() && x < map[y].size();
}

bool	in_map(Map& map, Plot& p)
{
	size_t	x = p.x;
	size_t	y = p.y;
	return y < map.size() && x < map[y].size();
}

std::vector<Plot>	get_neighbours(Map& map, Plot& p)
{
	static const std::vector<Plot>	diffs =
	{
		{0, -1},
		{0, +1},
		{+1, 0},
		{-1, 0}
	};
	std::vector<Plot>	neighbours {};
	for (const auto& diff : diffs)
	{
		int	x = p.x + diff.x;
		int	y = p.y + diff.y;
		if (in_map(map, Plot(x, y)) && map[y][x] == p.type)
		{
			neighbours.push_back(Plot(x, y, p.type));
		}
	}
	return neighbours;
}

void	check_fencing(Plot& visiting, std::vector<Plot>& neighbours)
{
	static const std::vector<Plot>	diffs =
	{
		{0, -1},
		{0, +1},
		{+1, 0},
		{-1, 0}
	};
	for (const auto& neighbour : neighbours)
	{
		Point	diff = neighbour - visiting;
		if (diff == diffs[NORTH])
			visiting.fenced[NORTH] = false;
		else if (diff == diffs[SOUTH])
			visiting.fenced[SOUTH] = false;
		else if (diff == diffs[EAST])
			visiting.fenced[EAST] = false;
		else if (diff == diffs[WEST])
			visiting.fenced[WEST] = false;
	}
}

void	clear_region_on_map(Map& map, std::set<Plot>& region)
{
	for (const auto& plot : region)
	{
		map[plot.y][plot.x] = 0;
	}
}

using HashSet = std::unordered_set<Plot,Point::Hasher>;
std::set<Plot>	find_region(Map& map, Plot p)
{
	std::set<Plot>		region {};
	std::deque<Plot>	searching {};
	HashSet				visited {};
	searching.push_back(p);
	while (!searching.empty())
	{
		Plot	visiting = searching.front();
		searching.pop_front();
		std::vector<Plot>	neighbours = get_neighbours(map, visiting);
		check_fencing(visiting, neighbours);
		for (auto& neighbour : neighbours)
		{
			if (!visited.contains(neighbour)
			 && std::find(searching.begin(), searching.end(), neighbour) == searching.end())
			{
				searching.push_back(neighbour);
			}
		}
		region.insert(visiting);
		visited.insert(visiting);
	}
	return region;
}

bool	is_fenced(const Plot& plot, const Plot& prev_plot, const Direction& dir)
{
	return plot.fenced[dir] && prev_plot.fenced[dir] == false;
}

std::vector<Plot>	get(const std::set<Plot>& region, Direction dir, int start_pos)
{
	if (dir == NORTH || dir == SOUTH)
	{
		std::vector<Plot>	row {};
		std::copy_if(region.begin(), region.end(), std::back_inserter(row),
					[&](const Plot& p)
					{
						return p.y == start_pos;
					});
		return row;
	}
	else if (dir == EAST || dir == WEST)
	{
		std::vector<Plot>	column {};
		std::copy_if(region.begin(), region.end(), std::back_inserter(column),
					[&](const Plot& p)
					{
						return p.x == start_pos;
					});
		return column;
	}
	throw std::exception();
}

int	get_axis_diff(Direction dir, const Plot& curr, const Plot& prev)
{
	if (dir == NORTH || dir == SOUTH)
	{
		return curr.x - prev.x;
	}
	else
	{
		return curr.y - prev.y;
	}
}

int	find_sides(const std::set<Plot>& region, Direction dir)
{
	int	sides {0};
	int	start_pos, end_pos;
	std::function<bool(const Point&, const Point&)> cmp;
	if (dir == NORTH || dir == SOUTH)
	{
		start_pos = region.begin()->y;
		end_pos = region.rbegin()->y + 1;
		cmp = [](const Point& p1, const Point& p2) { return p1.x < p2.x; };
	}
	else if (dir == EAST || dir == WEST)
	{
		start_pos = region.begin()->x;
		end_pos = region.rbegin()->x + 1;
		cmp = [](const Point& p1, const Point& p2) { return p1.y < p2.y; };
	}
	while (start_pos < end_pos)
	{
		std::vector<Plot>	axis_plots = get(region, dir, start_pos);
		std::sort(axis_plots.begin(), axis_plots.end(), cmp);
		Plot				prev = axis_plots.front();
		for (const Plot& plot : axis_plots)
		{
			if ((plot.fenced[dir] && prev.fenced[dir] != plot.fenced[dir])
				|| (plot.fenced[dir] && plot == axis_plots[0])
				|| (plot.fenced[dir] && get_axis_diff(dir, plot, prev)))
			{
				++sides;
			}
			prev = plot;
		}
		++start_pos;
	}
	return sides;
}

uint64_t	find_north_side(int y_start, int y_end, std::set<Plot> region)
{
	uint64_t	sides {0};

	while (y_start < y_end + 1)
	{
		std::vector<Plot>	row {};
		std::copy_if(region.begin(),
					 region.end(),
					 std::back_inserter(row),
					 [&](const Plot& p)
					{
						return p.y == y_start;
					});
		std::sort(row.begin(), row.end(), [](const Plot& p1, const Plot& p2)
		{
			return p1.x < p2.x;
		});
		auto	c = row.begin();
		auto 	p = c;
		std::vector<Plot>::iterator	curr = row.begin();
		std::vector<Plot>::iterator	prev = curr;
		while (curr != row.end())
		{
			if ((c->fenced[NORTH] && p->fenced[NORTH] != c->fenced[NORTH]) ||
				(c->fenced[NORTH] && c == row.begin()) ||
				(c->fenced[NORTH] && c->x - p->x > 1))
			{
				++sides;
			}
			prev = curr;
			++curr;
		}
		++y_start;
	}
	std::cout << "Found " << sides << " north sides\n";
	return sides;
}

uint64_t	find_south_side(int y_start, int y_end, std::set<Plot> region)
{
	uint64_t	sides {0};

	while (y_start < y_end + 1)
	{
		std::vector<Plot>	row {};
		std::copy_if(region.begin(),
					 region.end(),
					 std::back_inserter(row),
					 [&](const Plot& p)
					{
						return p.y == y_start;
					});
		std::sort(row.begin(), row.end(), [](const Plot& p1, const Plot& p2)
		{
			return p1.x < p2.x;
		});
		auto	c = row.begin();
		auto 	p = c;
		while (c != row.end())
		{
			if ((c->fenced[SOUTH] && p->fenced[SOUTH] != c->fenced[SOUTH]) ||
				(c->fenced[SOUTH] && c == row.begin()) ||
				(c->fenced[SOUTH] && c->x - p->x > 1))
			{
				++sides;
			}
			p = c;
			++c;
		}
		++y_start;
	}
	std::cout << "Found " << sides << " south sides\n";
	return sides;
}

uint64_t	find_east_side(int x_start, int x_end, std::set<Plot> region)
{
	uint64_t	sides {0};

	while (x_start < x_end+1)
	{
		std::vector<Plot>	column {};
		std::copy_if(region.begin(),
					 region.end(),
					 std::back_inserter(column),
					 [&](const Plot& p)
					{
						return p.x == x_start;
					});
		std::sort(column.begin(), column.end(), [](const Plot& p1, const Plot& p2)
		{
			return p1.y < p2.y;
		});
		auto	c = column.begin();
		auto 	p = c;
		while (c != column.end())
		{
			if ((c->fenced[EAST] && p->fenced[EAST] != c->fenced[EAST]) ||
				(c->fenced[EAST] && c == column.begin()) ||
				(c->fenced[EAST] && c->y - p->y > 1))
			{
				++sides;
			}
			p = c;
			++c;
		}
		++x_start;
	}
	std::cout << "Found " << sides << " east sides\n";
	return sides;
}

uint64_t	find_west_side(int x_start, int x_end, std::set<Plot> region)
{
	uint64_t	sides {0};

	while (x_start < x_end+1)
	{
		std::vector<Plot>	column {};
		std::copy_if(region.begin(),
					 region.end(),
					 std::back_inserter(column),
					 [&](const Plot& p)
					{
						return p.x == x_start;
					});
		std::sort(column.begin(), column.end(), [](const Plot& p1, const Plot& p2)
		{
			return p1.y < p2.y;
		});
		auto c = column.begin();
		auto p = c;
		while (c != column.end())
		{
			if ((c->fenced[WEST] && p->fenced[WEST] != c->fenced[WEST]) ||
				(c->fenced[WEST] && c == column.begin()) ||
				(c->fenced[WEST] && c->y - p->y > 1))
			{
				++sides;
			}
			p = c;
			++c;
		}
		++x_start;
	}
	std::cout << "Found " << sides << " west sides\n";
	return sides;
}

uint64_t	calculate_region_price(Map& map, Point p)
{
	char				plant_type = map[p.y][p.x];
	std::set<Plot>		region = find_region(map, Plot(p, plant_type));
	uint64_t			total_area = region.size();
	uint64_t			total_sides {0};

	int	x_start = std::numeric_limits<int>::max();
	std::for_each(region.begin(), region.end(), [&](const Plot& p)
	{
		if (p.x < x_start)
			x_start = p.x;
	});
	int	x_end = std::numeric_limits<int>::min();
	std::for_each(region.begin(), region.end(), [&](const Plot& p)
	{
		if (p.x > x_end)
			x_end = p.x;
	});
	int	y_start = std::numeric_limits<int>::max();
	std::for_each(region.begin(), region.end(), [&](const Plot& p)
	{
		if (p.y < y_start)
			y_start = p.y;
	});
	int	y_end = std::numeric_limits<int>::min();
	std::for_each(region.begin(), region.end(), [&](const Plot& p)
	{
		if (p.y > y_end)
			y_end = p.y;
	});

	total_sides += find_north_side(y_start, y_end, region);
	total_sides += find_south_side(y_start, y_end, region);
	total_sides += find_east_side(x_start, x_end, region);
	total_sides += find_west_side(x_start, x_end, region);

	// total_sides += find_sides(region, NORTH);
	// total_sides += find_sides(region, SOUTH);
	// total_sides += find_sides(region, EAST);
	// total_sides += find_sides(region, WEST);
	clear_region_on_map(map, region);
	std::cout << "Region " << plant_type << ": "
			  << total_area << " area * "
			<< total_sides << " sides\n";
	return total_area * total_sides;
}

int	main()
{
	Map	map = parse("example5.txt");

	uint64_t	total_price {0};
	for (size_t row = 0; row < map.size(); ++row)
	{
		for (size_t column = 0; column < map[row].size(); ++column)
		{
			if (map[row][column] != 0)
			{
				total_price += calculate_region_price(map, Point(column, row));
			}
		}
	}
	std::cout << "Total price is " << total_price << std::endl;
}