#include "Antenna.hpp"
#include "utility"

Antenna::Antenna(int x, int y, char frequency):
x(x),
y(y),
frequency(frequency)
{

}

bool    Antenna::same_frequency(const Antenna& a1, const Antenna& a2)
{
    return a1.frequency == a2.frequency;
}

std::pair<int, int>
Antenna::get_distance(const Antenna& a1, const Antenna& a2)
{
    int dx = a2.x - a1.x;
    int dy = a2.y - a1.y;
    return std::make_pair(dx, dy);
}

std::pair<std::size_t, std::size_t>
Antenna::calculate_antinode(std::pair<int, int> distance) const
{
    std::size_t new_x = x + distance.first; 
    std::size_t new_y = y + distance.second;
    return std::make_pair(new_x, new_y);
}