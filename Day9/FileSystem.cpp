#include <memory>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "FileSystem.hpp"

FileSystem::FileSystem():
data{},
file_count {0}
{
}

void    FileSystem::insert_file(int size)
{
    data.push_back(std::make_unique<File>(file_count, size));
    ++file_count;
}

void    FileSystem::insert_freespace(int size)
{
    data.push_back(std::make_unique<FreeSpace>(size));
}

void    FileSystem::visualize()
{
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		it->get()->visualize();
	}
    std::cout << std::endl;
}

void    FileSystem::defragment()
{
    auto file_it = get_last_file();
    auto space_it = get_first_free_space();

    while (space_it != data.end() && file_it != data.end())
    {
        File*		file = dynamic_cast<File*>(file_it->get());
		FreeSpace*	space = dynamic_cast<FreeSpace*>(space_it->get());

		if (file && space)
		{
			if (file->size < space->size)
			{
				data.insert(space_it,
				std::make_unique<File>(file->id, file->size));
				space_it = get_first_free_space();
				space->resize(space->size - file->size);
				file_it = get_last_file();
				file_it = data.erase(file_it);
			}
			else if (file->size > space->size)
			{
				size_t	split_file_size = space->size;
				data.insert(space_it,
				std::make_unique<File>(file->id, split_file_size));
				space_it = get_first_free_space();
				file_it = get_last_file();
				file->resize(file->size - split_file_size);
				space_it = data.erase(space_it);
			}
			else
			{
				data.insert(space_it,
				std::make_unique<File>(file->id, file->size));
				file_it = get_last_file();
				space_it = get_first_free_space();
				file_it = data.erase(file_it);
				space_it = data.erase(space_it);
			}
			// visualize();
			file_it = get_last_file();
			space_it = get_first_free_space();
		}
	}
}

std::vector<std::unique_ptr<Block>>::iterator   
FileSystem::get_first_free_space()
{
    return std::find_if(data.begin(), data.end(),
    [](const std::unique_ptr<Block>& b)
    {
        return dynamic_cast<FreeSpace*>(b.get());
    });
}

std::vector<std::unique_ptr<Block>>::iterator
FileSystem::get_last_file()
{
    auto rit = std::find_if(data.rbegin(), data.rend(),
    [](const std::unique_ptr<Block>& b)
    {
        return dynamic_cast<File*>(b.get());
    });
    if (rit == data.rend())
        return data.end();
    return std::prev(rit.base());
}

uint64_t	FileSystem::calculate_checksum()
{
	// convert chunks of blocks into individual blocks
	std::vector<uint64_t>	blocks;
	for (size_t i = 0; i < data.size(); ++i)
	{
		File*	file = dynamic_cast<File*>(data[i].get());
		FreeSpace*	space = dynamic_cast<FreeSpace*>(data[i].get());
		if (file)
		{
			for (size_t j = 0; j < file->size; j++)
			{
				blocks.push_back(file->id);
			}
		}
		else
		{
			for (size_t j = 0; j < space->size; j++)
			{
				blocks.push_back(-1);
			}
		}
	}
	uint64_t	sum {0};
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		int	checksum = blocks[i] * i;
		sum += checksum;
		// File*	file = dynamic_cast<File*>(data[i].get());
		// if (file)
		// {
		// 	int	checksum = file->id * i;
		// 	sum += checksum;
		// }
	}
	return sum;
}