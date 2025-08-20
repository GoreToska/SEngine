//
// Created by gorev on 13.08.2025.
//

#ifdef max
#undef max
#endif
#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H
#include <string>
#include <algorithm>

inline std::wstring ToWideString(const std::string& str)
{
    return std::wstring(str.begin(), str.end());
}

inline std::string GetDirectory(const std::string& path)
{
    size_t pos1 = path.find_last_of("\\");
    size_t pos2 = path.find_last_of("/");
    if (pos1 == std::string::npos && pos2 == std::string::npos)
        return "";

    if (pos1 == std::string::npos)
    {
        return path.substr(0, pos2 - 1);
    }
    if (pos2 == std::string::npos)
    {
        return path.substr(0, pos1 - 1);
    }

    return path.substr(0, std::max(pos1, pos2) - 1);
}

inline std::string GetExtension(const std::string& fileName)
{
    size_t pos = fileName.find_last_of(".");
    if (pos != std::string::npos)
        return {};

    return fileName.substr(pos + 1);
}

#endif //STRINGUTILITIES_H
