#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <vector>
#include <cstddef>
#include <functional>

using std::size_t;

class Point
{
public:
    Point() = default;
    ~Point() = default;
    Point(int, int);

    bool	operator==(const Point&) const;
	bool	operator!=(const Point&) const;
	bool	operator<(const Point&) const;
	bool	operator>(const Point&) const;
	Point&	operator+=(const Point&);
	Point	operator+(const Point&) const;
	Point&	operator-=(const Point&);
	Point	operator-(const Point&) const;
	friend std::ostream&	operator<<(std::ostream&, const Point&);

	struct Hasher
	{
		size_t	operator()(const Point& p) const;
	};

	static Point	find_in(std::vector<std::vector<char>>, const char&);
	static uint64_t	manhattan_dist(const Point&, const Point&);
	static int		euclidean_dist(const Point&, const Point&);

    int x;
    int y;
};

#endif