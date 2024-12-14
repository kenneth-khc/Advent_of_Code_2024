#ifndef GUARD_HPP
# define GUARD_HPP

#include <vector>
#include <string>
#include "Map.hpp"
#include "Point.hpp"

class Guard
{
private:
	Point	pos;	
    int 	unique_tiles_travelled;
    int 	step_x;
    int 	step_y;
	bool	escaped;
     
public:
	Guard(/* args */) = default;
    Guard(int, int);
	~Guard() = default;
	Guard(const Guard&) = default;

    void    move(Map&);
	bool	in_map() const;
    void    change_direction();
	Point	get_pos() const;
};

#endif