#include <fstream>
#include <iostream>
#include <vector>
#include <optional>
#include <memory>
#include "File.hpp"
#include "FreeSpace.hpp"
#include "FileSystem.hpp"

std::string file_to_str(std::string filename)
{
    std::fstream    file {filename};
    std::string     str;
    while (file >> str)
        ;
    return str;
}

FileSystem  create_filesystem(std::string& line)
{
    FileSystem  filesystem;

    for (size_t i = 0; i < line.size(); ++i)
    {
        int count;
        if (i % 2 == 0)
        {
            count = static_cast<int>(line[i] - '0');
            filesystem.insert_file(count);
        }
        else
        {
            count = static_cast<int>(line[i] - '0');
            filesystem.insert_freespace(count);
        }
    }
    return filesystem;
}

int main()
{
    std::string line = file_to_str("input.txt");
    FileSystem  filesystem = create_filesystem(line);
    // filesystem.visualize();
    filesystem.defragment();
    uint64_t checksum = filesystem.calculate_checksum();
    std::cout << "Sum of checksums is " << checksum << std::endl;
}