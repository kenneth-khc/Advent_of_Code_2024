#ifndef ANTINODE_HPP
# define ANTINODE_HPP

class Antinode
{
private:
    int x;
    int y;

public:
    Antinode(/* args */) = default;
    Antinode(int, int); 
    ~Antinode() = default;
    bool operator==(const Antinode&) const;
};

#endif