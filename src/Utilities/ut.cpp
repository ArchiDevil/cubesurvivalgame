#include "ut.h"
#include "logger.h"

#if defined (WIN32) || (_WIN32)
    #include <Windows.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdlib>

std::string utils::WStrToStr( const std::wstring & str )
{
	return std::string(str.begin(), str.end());
}

std::wstring utils::StrToWStr( const std::string & str )
{
	return std::wstring(str.begin(), str.end());
}

std::vector<std::wstring> utils::filesystem::CollectFileNames(const std::wstring & Path, const std::wstring & Extension /*= L"*"*/)
{
#if defined (WIN32) || (_WIN32)
	std::wstring bufferPath = Path;
	//check path here
	if(bufferPath[bufferPath.size() - 1] != L'\\')
		bufferPath.push_back(L'\\');

	std::vector<std::wstring> names;
	WIN32_FIND_DATAW data;
	HANDLE file;
	std::wstring fName = std::wstring(bufferPath + L"*." + Extension);
	file = FindFirstFile(fName.c_str(), &data);
	if(file == INVALID_HANDLE_VALUE)
	{

		LOG_ERROR("Unable to get filenames from " + utils::WStrToStr(bufferPath)
			+ ", may be directory is not exist or it's empty.");
		return std::vector<std::wstring>();
	}

	std::wstring buffer = data.cFileName;
	if(buffer != L"." && buffer != L"..")
	{
		names.push_back(buffer);
	}

	while(FindNextFile(file, &data))
	{
		buffer = data.cFileName;
		if(buffer != L"." && buffer != L"..")
		{
			names.push_back(buffer);
		}
	}
	FindClose(file);
	return names;
#else
    return std::vector<std::wstring> ();
#endif
}

std::wstring utils::ExtractName( const std::wstring & filename )
{
	auto iter = std::find(filename.begin(), filename.end(), '.');
	return std::wstring(filename.begin(), iter);
}

std::wstring utils::ExtractExtension( const std::wstring & filename )
{
	std::wstring buffer = filename;
	int pos = buffer.find(L".");
	if(pos != -1)
		buffer = buffer.substr(pos + 1, buffer.size());
	else
		return std::wstring(L"");

	for (;;)
	{
		pos = buffer.find(L".");
		if(pos == -1)
			break;
		buffer = buffer.substr(pos + 1, buffer.size());
	}

	return buffer;
}

bool utils::IsNumber( const std::string & str )
{
	return std::all_of(str.cbegin(), str.cend(), ::isdigit);
}

bool utils::IsNumber( const std::wstring & str )
{
	return std::all_of(str.cbegin(), str.cend(), ::isdigit);
}

bool utils::filesystem::CreateDir( const std::wstring & directoryName )
{
#ifdef WIN32
	return ::CreateDirectory(directoryName.c_str(), NULL) != 0;
#else
	return false;
#endif
}

bool utils::filesystem::IsDirectory(const std::wstring & directoryName)
{
#ifdef WIN32
	DWORD ftyp = GetFileAttributes(directoryName.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
#else
	return false;
#endif
}
