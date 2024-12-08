#ifndef ANTENNA_HPP
# define ANTENNA_HPP

#include <utility>

class Antenna
{
private:
    int		x;
	int		y;
    char	frequency;

public:
    Antenna(/* args */) = default;
	Antenna(int, int, char);
    ~Antenna() = default;

	std::pair<std::size_t, std::size_t>
	calculate_antinode(std::pair<int, int>) const;

	static bool same_frequency(const Antenna&, const Antenna&);
	static std::pair<int, int>	get_distance(const Antenna&, const Antenna&);
};

#endif