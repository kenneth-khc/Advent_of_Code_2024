#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <vector>
#include <cstddef>
#include <functional>

using std::size_t;

template<typename Scalar = int>
class Point
{
public:

    Point() = default;
    ~Point() = default;
	Point(const Point&) = default;
	Point(Point&&) = default;
    Point(Scalar, Scalar);
    bool	operator==(const Point&) const;
	bool	operator!=(const Point&) const;
	bool	operator<(const Point&) const;
	bool	operator>(const Point&) const;
	Point&	operator+=(const Point&);
	Point	operator+(const Point&) const;
	Point&	operator-=(const Point&);
	Point	operator-(const Point&) const;
	friend std::ostream&	operator<<(std::ostream&, const Point<>&);
	Point&	operator=(const Point&) = default;

	struct Hasher
	{
		size_t	operator()(const Point&) const;
		size_t	operator()(const std::pair<Point,Point>&) const;
	};

	static Point	find_in(std::vector<std::vector<char>>, const char&);
	static uint64_t	manhattan_dist(const Point&, const Point&);
	static int		euclidean_dist(const Point&, const Point&);

    Scalar	x;
    Scalar	y;
};

#include "Point.tpp"

#endif