#ifndef GUARD_HPP
# define GUARD_HPP

#include <vector>
#include <string>
#include "Map.hpp"

class Guard
{
private:
    int x;
    int y;
    int unique_tiles_travelled;
    int step_x;
    int step_y;
     

public:
	Guard(/* args */) = default;
    Guard(int, int);
	~Guard() = default;

    void    move(Map&);
    void    change_direction();
};

#endif