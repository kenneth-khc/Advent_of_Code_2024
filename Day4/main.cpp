#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>

#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOT_LEFT 2
#define BOT_RIGHT 3

class Point
{
public:
	size_t	x;
	size_t	y;
};

bool	check_diagonal(std::vector<std::string>& lines,
size_t x, size_t y, bool left)
{
	Point	p1;
	Point	p2;

	if (left)
	{
		p1.x = x - 1; p1.y = y - 1;
		p2.x = x + 1; p2.y = y + 1;
	}
	if (!left)
	{
		p1.x = x + 1; p1.y = y - 1;
		p2.x = x - 1; p2.y = y + 1;
	}
	if (p1.y >= lines.size() || p2.y >= lines.size()
		|| p1.x >= lines[0].size() || p2.x >= lines[0].size())
	{
		return false;
	}
	return (lines[p1.y][p1.x] == 'M' && lines[p2.y][p2.x] == 'S') ||
			(lines[p1.y][p1.x] == 'S' && lines[p2.y][p2.x] == 'M');
}

int	find_xmas(std::vector<std::string>& lines,
size_t x, size_t y)
{
	bool	left_diagonal = check_diagonal(lines, x, y, true);
	bool	right_diagonal = check_diagonal(lines, x, y, false);

	return left_diagonal && right_diagonal;
}

int main()
{
    std::fstream				file{"input.txt"};
	std::vector<std::string>	lines;
	int							count{0};

	while (!file.eof())
	{
		std::string	buffer;
		std::getline(file, buffer);
		lines.push_back(buffer);
	}

	for (size_t y = 0;  y < lines.size(); y++)
	{
		for (size_t x = 0; x < lines[y].size(); x++)
		{
			if (lines[y][x] == 'A')
			{
				count += find_xmas(lines, x, y);
			}
		}
	}
	std::cout << "Counted " << count << " xmas" << std::endl;
}