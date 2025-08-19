//
// Created by gorev on 15.08.2025.
//

#ifndef FILESYSTEMUTILITIES_H
#define FILESYSTEMUTILITIES_H
#include <filesystem>
#include <string>


inline bool DirectoryContains(const std::string& file, const std::filesystem::path& path)
{
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (const auto& entry: std::filesystem::directory_iterator(path))
        {
            if (entry.is_regular_file() && entry.path().stem() == file)
            {
                return true;
            }
        }
    }

    return false;
}


#endif //FILESYSTEMUTILITIES_H
