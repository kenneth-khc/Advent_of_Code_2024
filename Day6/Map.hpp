#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>

class Map
{
private:
    std::vector<std::string>    layout;
    size_t  width;
    size_t  height;
public:
    Map(/* args */) = default;
    ~Map() = default;
    void    add(std::string);
    void    print();
    void    mark(size_t, size_t);
	size_t	get_width();
	size_t	get_height();

	char&	operator()(size_t x, size_t);
};

#endif