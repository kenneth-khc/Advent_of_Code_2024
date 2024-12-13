#include <fstream>
#include <iostream>
#include <cmath>

struct Point
{
	uint64_t	x;
	uint64_t	y;
	
	Point() = default;
	Point(uint64_t x, uint64_t y): x(x), y(y) { }
};

Point	parse_point(std::string& line, std::string match);
int64_t	determinant(Point a, Point b);
bool	is_whole(double f);
bool	is_valid(Point& button_a, Point& button_b, 
		double a_presses, double b_presses, Point& target);

int	main()
{
	uint64_t		sum_cost {0};
	std::ifstream	file {"input.txt"};
	while (file)
	{
		std::string	line;
		std::getline(file, line);
		Point a_step = parse_point(line, "+");
		std::getline(file, line);
		Point b_step = parse_point(line, "+");
		std::getline(file, line);
		Point target = parse_point(line, "=");
		std::getline(file, line);

		int64_t	det = determinant(a_step, b_step);
		int64_t	det_a = determinant(target, b_step);
		int64_t	det_b = determinant(a_step, target);
		double	a_press_count = det_a / det;
		double	b_press_count = det_b / det;
		if (!is_whole(a_press_count) || !is_whole(b_press_count) || 
			!is_valid(a_step, b_step, a_press_count, b_press_count, target))
			continue ;

		uint64_t	a_cost = a_press_count * 3;
		uint64_t	b_cost = b_press_count * 1;
		sum_cost += a_cost + b_cost;
	}
	std::cout << "Sum of all costs is " << sum_cost << std::endl;
}

Point	parse_point(std::string& line, std::string match)
{
	size_t	x_pos = line.find("X" + match) + 2;
	size_t	y_pos = line.find("Y" + match) + 2;

	uint64_t	x = std::stoull(&line[x_pos]);
	uint64_t	y = std::stoull(&line[y_pos]);
	if (match == "=")
	{
		x += 10000000000000;
		y += 10000000000000;
	}
	return Point(x, y);
}

int64_t	determinant(Point a, Point b)
{
	int64_t	ad = a.x * b.y;
	int64_t	bc = a.y * b.x;
	int64_t	diff = ad - bc;

	return diff;
}

bool	is_whole(double f)
{
	return std::ceil(f) == f;
}

bool	is_valid(Point& button_a, Point& button_b, 
double a_count, double b_count, Point& target)
{
	return
	(a_count * button_a.x + b_count * button_b.x == target.x) &&
	(a_count * button_a.y + b_count * button_b.y == target.y);
}