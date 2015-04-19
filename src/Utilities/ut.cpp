#include "ut.h"

#include "logger.hpp"

#if defined (WIN32) || (_WIN32)
    #include <Windows.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdlib>

extern Log MainLog("Application.log");

std::string utils::Narrow(const std::wstring & str)
{
    return std::string(str.begin(), str.end());
}

std::wstring utils::Widen(const std::string & str)
{
    return std::wstring(str.begin(), str.end());
}

std::vector<std::string> utils::filesystem::CollectFileNames(const std::string & Path, const std::string & Extension)
{
#if defined (WIN32) || (_WIN32)
    std::string bufferPath = Path;
    //check path here
    if (bufferPath[bufferPath.size() - 1] != '\\' && bufferPath[bufferPath.size() - 1] != '/')
        bufferPath.push_back('/');

    std::vector<std::string> names;
    WIN32_FIND_DATAA data;
    HANDLE file;
    std::string fName = std::string(bufferPath + "*." + Extension);
    file = FindFirstFileA(fName.c_str(), &data);
    if (file == INVALID_HANDLE_VALUE)
    {
        LOG_ERROR("Unable to get filenames from ", bufferPath, ", may be directory is not exist or it's empty.");
        return std::vector<std::string>();
    }

    std::string buffer = data.cFileName;
    if (buffer != "." && buffer != "..")
    {
        names.push_back(buffer);
    }

    while (FindNextFileA(file, &data))
    {
        buffer = data.cFileName;
        if (buffer != "." && buffer != "..")
        {
            names.push_back(buffer);
        }
    }
    FindClose(file);
    return names;
#else
    LOG_ERROR("Not implemented yet.");
    return std::vector<std::string>();
#endif
}

std::vector<std::wstring> utils::filesystem::CollectFileNames(const std::wstring & Path, const std::wstring & Extension /*= L"*"*/)
{
#if defined (WIN32) || (_WIN32)
    std::wstring bufferPath = Path;
    //check path here
    if (bufferPath[bufferPath.size() - 1] != L'\\' && bufferPath[bufferPath.size() - 1] != L'/')
        bufferPath.push_back(L'\\');

    std::vector<std::wstring> names;
    WIN32_FIND_DATAW data;
    HANDLE file;
    std::wstring fName = std::wstring(bufferPath + L"*." + Extension);
    file = FindFirstFileW(fName.c_str(), &data);
    if (file == INVALID_HANDLE_VALUE)
    {
        LOG_ERROR("Unable to get filenames from ", utils::Narrow(bufferPath), ", may be directory is not exist or it's empty.");
        return std::vector<std::wstring>();
    }

    std::wstring buffer = data.cFileName;
    if (buffer != L"." && buffer != L"..")
    {
        names.push_back(buffer);
    }

    while (FindNextFileW(file, &data))
    {
        buffer = data.cFileName;
        if (buffer != L"." && buffer != L"..")
        {
            names.push_back(buffer);
        }
    }
    FindClose(file);
    return names;
#else
    LOG_ERROR("Not implemented yet.");
    return std::vector<std::wstring>();
#endif
}

std::string utils::ExtractName(const std::string & filename)
{
    std::string::size_type pos = filename.rfind('.');
    if (pos == filename.npos)
        return filename;
    else
        return filename.substr(0, pos);
}

std::wstring utils::ExtractName(const std::wstring & filename)
{
    std::wstring::size_type pos = filename.rfind(L'.');
    if (pos == filename.npos)
        return filename;
    else
        return filename.substr(0, pos);
}

std::string utils::ExtractExtension(const std::string & filename)
{
    std::string::size_type pos = filename.rfind('.');
    if (pos == filename.npos)
        return std::string();
    else
        return filename.substr(pos + 1, filename.npos);
}

std::wstring utils::ExtractExtension(const std::wstring & filename)
{
    std::wstring::size_type pos = filename.rfind(L'.');
    if (pos == filename.npos)
        return std::wstring();
    else
        return filename.substr(pos + 1, filename.npos);
}

bool utils::IsNumber(const std::string & str)
{
    if (str.empty())
        return false;
    return std::all_of(str.cbegin(), str.cend(), ::isdigit);
}

bool utils::IsNumber(const std::wstring & str)
{
    if (str.empty())
        return false;
    return std::all_of(str.cbegin(), str.cend(), ::isdigit);
}

bool utils::filesystem::CreateDir(const std::string & directoryName)
{
#ifdef WIN32
    return ::CreateDirectoryA(directoryName.c_str(), NULL) != 0;
#else
    LOG_ERROR("Not implemented yet.");
    return false;
#endif
}

bool utils::filesystem::CreateDir(const std::wstring & directoryName)
{
#ifdef WIN32
    return ::CreateDirectoryW(directoryName.c_str(), NULL) != 0;
#else
    LOG_ERROR("Not implemented yet.");
    return false;
#endif
}

bool utils::filesystem::IsDirectory(const std::string & directoryName)
{
#ifdef WIN32
    DWORD ftyp = GetFileAttributesA(directoryName.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
#else
    LOG_ERROR("Not implemented yet.");
    return false;
#endif
}

bool utils::filesystem::IsDirectory(const std::wstring & directoryName)
{
#ifdef WIN32
    DWORD ftyp = GetFileAttributesW(directoryName.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
#else
    LOG_ERROR("Not implemented yet.");
    return false;
#endif
}

bool utils::filesystem::RemoveFile(const std::string & fileName)
{
    return DeleteFileA(fileName.c_str());
}

bool utils::filesystem::RemoveFile(const std::wstring & fileName)
{
    return DeleteFileW(fileName.c_str());
}
