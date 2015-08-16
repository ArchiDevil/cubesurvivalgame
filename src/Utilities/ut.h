#pragma once

#include <string>
#include <vector>

namespace utils
{
    std::string Narrow(const std::wstring & str);
    std::wstring Widen(const std::string & str);

    std::string ExtractExtension(const std::string & filename);
    std::wstring ExtractExtension(const std::wstring & filename);

    std::string ExtractName(const std::string & filename);
    std::wstring ExtractName(const std::wstring & filename);

    bool IsNumber(const std::string & str);
    bool IsNumber(const std::wstring & str);

    namespace filesystem
    {
        bool CreateDir(const std::string & directoryName);
        bool CreateDir(const std::wstring & directoryName);

        bool IsDirectory(const std::string & directoryName);
        bool IsDirectory(const std::wstring & directoryName);

        bool DeleteDir(const std::string & directoryName);
        bool DeleteDir(const std::wstring & directoryName);

        bool RemoveFile(const std::string & fileName);
        bool RemoveFile(const std::wstring & fileName);

        std::vector<std::string> CollectFileNames(const std::string & Path, const std::string & Extension = "*");
        std::vector<std::wstring> CollectFileNames(const std::wstring & Path, const std::wstring & Extension = L"*");
    }
}
