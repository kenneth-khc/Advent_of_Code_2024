#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <vector>
#include <memory>
#include "Block.hpp"
#include "File.hpp"
#include "FreeSpace.hpp"

class FileSystem
{
private:
using BlockVector = std::vector<std::unique_ptr<Block>>;
BlockVector data;
size_t  file_count;

public:
    FileSystem(/* args */);
    FileSystem(FileSystem&&) = default;
    ~FileSystem() = default;
    void    insert_file(int);
    void    insert_freespace(int);
    void    visualize();
    void    defragment();
    std::vector<int64_t>   deblock();

	bool	has_movable(std::vector<int>&);

	std::vector<std::unique_ptr<Block>>::iterator
	get_file_it(int id);

	std::vector<std::unique_ptr<Block>>::iterator
	get_first_free_space();

	std::vector<std::unique_ptr<Block>>::iterator
	get_suitable_space(const std::unique_ptr<Block>&);

	std::vector<std::unique_ptr<Block>>::iterator
	get_last_file();

    std::optional<size_t>	first_space();
	std::optional<size_t>	last_file();

	uint64_t	calculate_checksum();
};

#endif