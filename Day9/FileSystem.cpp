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

using BlockIt = std::vector<std::unique_ptr<Block>>::iterator;

#include <limits>

bool	FileSystem::has_movable(std::vector<int>& moved_files)
{
	// int	smallest_file_size = std::find_if(data.begin(), data.end(),
	// [&](const std::unique_ptr<Block>& b){
	// 	static int smallest = std::numeric_limits<int>::max();
	// 	File	*file = dynamic_cast<File*>(b.get());
	// 	if (file)
	// 	{
	// 		if (file->size < smallest_file_size)
	// 			smallest_file_size = file->size;
	// 	}
	// });
	size_t	smallest_file_size = std::numeric_limits<size_t>::max();
	for (size_t i = 0; i < data.size(); ++i)
	{
		File*	file = dynamic_cast<File*>(data[i].get());

		std::vector<int>::iterator	file_moved;
		if (file)
		{
			file_moved = std::find(moved_files.begin(), moved_files.end(), file->id);
		}

		if (file && file_moved == moved_files.end() && file->size < smallest_file_size)
			smallest_file_size = file->size;
	}
	for (size_t i = 0; i < data.size(); ++i)
	{
		FreeSpace*	space = dynamic_cast<FreeSpace*>(data[i].get());

		if (space && space->size >= smallest_file_size)
		{
			return true;
		}
	}
	return false;
}

void    FileSystem::defragment()
{
    BlockIt file_it = data.begin();
    BlockIt space_it = data.begin();
	// visualize();
	std::vector<int>	moved_files;
    // while (space_it != data.end())
	// while (file_count > 0)
	while (has_movable(moved_files))
    {
		int	file_id = static_cast<int>(file_count) - 1;
		while (file_id >= 0)
		{
			if (std::find(moved_files.begin(), moved_files.end(), file_id) == moved_files.end())
			{
				file_it = get_file_it(file_id);
				if (file_it != data.end())
				{
					space_it = get_suitable_space(*file_it);
					if (space_it != data.end())
						break;
				}
			}
			file_id--;
		}
        File*		file = dynamic_cast<File*>(file_it->get());
		FreeSpace*	space = dynamic_cast<FreeSpace*>(space_it->get());
		if (file && space)
		{
			if (file->size < space->size)
			{
				data.insert(space_it,
				std::make_unique<File>(file->id, file->size));
				moved_files.push_back(file_id);
				// space_it = get_first_free_space();
				file_it = get_file_it(file_id);
				space_it = get_suitable_space(*file_it);
				space->resize(space->size - file->size);
				file_it = get_file_it(file_id);
				data.insert(file_it,
				std::make_unique<FreeSpace>(file->size));
				file_it = get_file_it(file_id);
				file_it = data.erase(file_it);
				--file_count;
			}
			else
			{
				data.insert(space_it,
				std::make_unique<File>(file->id, file->size));
				moved_files.push_back(file_id);
				// space_it = get_first_free_space();
				file_it = get_file_it(file_id);
				space_it = get_suitable_space(*file_it);
				space_it = data.erase(space_it);
				file_it = get_file_it(file_id);
				data.insert(file_it,
				std::make_unique<FreeSpace>(file->size));
				file_it = get_file_it(file_id);
				file_it = data.erase(file_it);
				--file_count;
			}
			// visualize();
			int	file_id = static_cast<int>(file_count) - 1;
			while (file_id >= 0)
			{
				if (std::find(moved_files.begin(), moved_files.end(), file_id) == moved_files.end())
				{
					file_it = get_file_it(file_id);
					if (file_it != data.end())
					{
						space_it = get_suitable_space(*file_it);
						if (space_it != data.end())
							break;
					}
				}
				file_id--;
			}
		}
	}
}

std::vector<std::unique_ptr<Block>>::iterator
FileSystem::get_file_it(int id)
{
	for (int i = data.size() - 1; i >= 0; --i)
	{
		if (data[i]->id == id)
		{
			return data.begin() + i;
		}
	}
	return data.end();
}

std::vector<std::unique_ptr<Block>>::iterator
FileSystem::get_suitable_space(const std::unique_ptr<Block>& file)
{
	return std::find_if(data.begin(), data.end(),
	[&](const std::unique_ptr<Block>& b)
	{
		FreeSpace*	space = dynamic_cast<FreeSpace*>(b.get());

		return space && file.get()->size <= space->size;
	});
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

std::vector<int64_t>
FileSystem::deblock()
{
	std::vector<int64_t>	blocks;

	for (size_t i = 0; i < data.size(); ++i)
	{
		File*	file = dynamic_cast<File*>(data[i].get());
		if (file)
		{
			for (size_t j = 0; j < file->size; j++)
			{
				blocks.push_back(file->id);
			}
		}
		else
		{
			for (size_t j = 0; j < data[i].get()->size; j++)
			{
				blocks.push_back(-1);
			}
		}
	}
	return blocks;
}

uint64_t	FileSystem::calculate_checksum()
{
	// convert chunks of blocks into individual blocks
	std::vector<int64_t>	blocks = deblock();
	uint64_t	sum {0};
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		int	checksum {0};
		if (blocks[i] != -1)
			checksum = blocks[i] * i;
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

#if 0
std::optional<size_t>    FileSystem::first_space()
{
	size_t  index {0};

	while (index < data.size())
	{
		if (data[index].get()->get_type() == "FreeSpace")
		{
			return index;
		}
		++index;
	}
	return {};
}

std::optional<size_t>   FileSystem::last_file()
{
	size_t  index = data.size() - 1;

	while (index > 0)
	{
		if (data[index].get()->get_type() == "File")
		{
			return index;
		}
		--index;
	}
	return {};
}

void	FileSystem::defragment()
{
	std::optional<size_t>   file_idx = last_file();
	std::optional<size_t>   space_idx = first_space();

	while (file_idx && space_idx)
	{
		File*   file = dynamic_cast<File*>(data[*file_idx].get());
		FreeSpace*  space = dynamic_cast<FreeSpace*>(data[*space_idx].get());
		if (file->size < space->size)
		{
			data.insert(data.begin() + *space_idx,
				std::make_unique<File>(file->id, file->size));
			file_idx = last_file();
			space_idx = first_space();
			space->resize(space->size - file->size);
			data.erase(data.begin() + *file_idx);
		}
		else if (file->size > space->size)
		{
			size_t  fragmented_file_size = space->size;
			data.insert(data.begin() + *space_idx,
				std::make_unique<File>(file->id, fragmented_file_size));
			file_idx = last_file();
			space_idx = first_space();
			file->resize(file->size - fragmented_file_size);
			data.erase(data.begin() + *space_idx);
		}
		else
		{
			data.insert(data.begin() + *space_idx,
				std::make_unique<File>(file->id, file->size));
			file_idx = last_file();
			space_idx = first_space();
			data.erase(data.begin() + *space_idx);
			data.erase(data.begin() + *file_idx);
		}
		file_idx = last_file();
		space_idx = first_space();
	}
}
#endif