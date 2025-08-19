//
// Created by gorev on 13.08.2025.
//

#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H
#include <string>

inline std::wstring ToWideString(const std::string& str)
{
    return std::wstring(str.begin(), str.end());
}

#endif //STRINGUTILITIES_H
