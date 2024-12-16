#ifndef POINT_HPP
#define POINT_HPP

using std::size_t;

class Point
{
public:
    Point() = default;
    Point(int, int);
    Point(const Point&) = default;
    Point(Point&&) = default;
    ~Point() = default;

    bool	operator==(const Point&) const;
    struct Hasher
    {
        size_t  operator()(const Point&) const;
    };


    int x;
    int y;
};

#endif