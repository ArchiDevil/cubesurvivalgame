#pragma once

#include <string>
#include <vector>

namespace utils
{
	std::string WStrToStr(const std::wstring & str);
	std::wstring StrToWStr(const std::string & str);

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

        std::vector<std::string> CollectFileNames(const std::string & Path, const std::string & Extension = "*");
		std::vector<std::wstring> CollectFileNames(const std::wstring & Path, const std::wstring & Extension = L"*");
	}
}

//определяет клавиши для мышки
enum MouseKeys
{
	LButton,
	RButton
};

//определяет информацию о мыши, получаемую с модуля ввода
struct MouseInfo
{
	MouseInfo()
		: deltaX(0)
		, deltaY(0)
		, deltaZ(0)
		, absoluteX(0)
		, absoluteY(0)
		, clientX(0)
		, clientY(0) 
	{}

	MouseInfo(long _deltaX, long _deltaY, long _deltaZ, unsigned long _absoluteX, unsigned long _absoluteY, long _clientX, long _clientY)
		: deltaX(_deltaX)
		, deltaY(_deltaY)
		, deltaZ(_deltaZ)
		, absoluteX(_absoluteX)
		, absoluteY(_absoluteY)
		, clientX(_clientX)
		, clientY(_clientY) 
	{}

	long deltaX, deltaY, deltaZ;
	unsigned long absoluteX, absoluteY;
	long clientX, clientY;

	bool operator == (const MouseInfo & ref) const
	{
		return (this->absoluteX == ref.absoluteX) && (this->absoluteY == ref.absoluteY);
	};

	bool operator != (const MouseInfo & ref) const
	{
		return !((this->absoluteX == ref.absoluteX) && (this->absoluteY == ref.absoluteY));
	};
};
