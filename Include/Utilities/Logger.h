//
// Created by gorev on 13.08.2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <Windows.h>
#include <comdef.h>
#include <d3dcommon.h>
#include <filesystem>
#include <sstream>
#include <wrl/client.h>

// Shows error and info. Can take HRESULT and string or just string.
#define SERROR(...) slog::Error(__VA_ARGS__, __FILE__, __LINE__)

#define SSHADERERROR(...) slog::HandleShaderCompilationError(__VA_ARGS__, __FILE__, __LINE__)

#define SLOG(...) slog::Log(__VA_ARGS__, __FILE__, __LINE__)

namespace slog
{
    // Shows error and info. Do not use this method, use SERROR macro instead.
    inline void Error(const std::string& message, const char* file, int line)
    {
        std::string filename = std::filesystem::path(file).filename().string();
        std::string error_message = "Error: " + message + "\nFile: " + filename + "\nLine: " + std::to_string(line);
        MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
    };

    // Shows error and info. Do not use this method, use SERROR macro instead.
    inline void Error(const HRESULT result, const std::string& message, const char* file, int line)
    {
        _com_error error(result);
        const auto filename = std::filesystem::path(file).filename().c_str();
        std::wstringstream wss;
        wss << L"Error: " <<
                std::wstring(message.begin(), message.end()) <<
                L"\n" << error.ErrorMessage() << "\nFile: " << filename << "\nLine: " << line;
        MessageBoxW(NULL, wss.str().c_str(), L"Error", MB_ICONERROR);
    };

    // Shows error and info. Do not use this method, use SERROR macro instead.
    inline void Error(const HRESULT result, const std::wstring& message, const char* file, int line)
    {
        const _com_error error(result);
        const auto filename = std::filesystem::path(file).filename().c_str();
        std::wstringstream wss;
        wss << L"Error: " <<
                std::wstring(message.begin(), message.end()) <<
                L"\n" << error.ErrorMessage() << "\nFile: " << filename << "\nLine: " << line;
        MessageBoxW(NULL, wss.str().c_str(), L"Error", MB_ICONERROR);
    };

    inline void HandleShaderCompilationError(const HRESULT result, const Microsoft::WRL::ComPtr<ID3D10Blob>& blob,
                                             const char* file, int line)
    {
        if (FAILED(result))
        {
            if (blob)
            {
                std::string errorMessage(static_cast<const char*>(blob->GetBufferPointer()),
                                         blob->GetBufferSize());
                SERROR("Shader compile error: " + errorMessage);
            }
            else
            {
                SERROR("Unknown error occurred while compiling shader.");
            }
        }
    }

    inline void Log(const std::string& message, const char* file, int line)
    {
        std::string filename = std::filesystem::path(file).filename().string();
        std::string log_message = message + "\nFile: " + filename + "\nLine: " + std::to_string(line);
        MessageBoxA(NULL, log_message.c_str(), "Log", MB_OK | MB_ICONINFORMATION);
    }
}

#endif //LOGGER_H
