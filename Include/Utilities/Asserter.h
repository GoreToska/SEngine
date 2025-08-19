//
// Created by gorev on 13.08.2025.
//

#ifndef ASSERTER_H
#define ASSERTER_H

#define ThrowIfFailed(hr, msg) if(FAILED(hr)) throw SException(hr, msg, __FILE__, __FUNCTION__, __LINE__)
#include <comdef.h>
#include <intsafe.h>
#include <string>

#include "StringUtilities.h"

class SException
{
public:
    SException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, int line)
    {
        _com_error error(hr);
        std::wstringstream wss;
        wss << L"Message: " << ToWideString(std::string(msg)) << L"\n" <<
                error.ErrorMessage() << L"\nFile: " << ToWideString(std::string(file)) <<
                L"\nFunction: " << ToWideString(std::string(function)) <<
                L"\nLine: " << ToWideString(std::to_string(line));
        errorMsg = wss.str();
    }

    const wchar_t* ErrorMsg() const
    {
        return errorMsg.c_str();
    }

private:
    std::wstring errorMsg;
};

#endif //ASSERTER_H
