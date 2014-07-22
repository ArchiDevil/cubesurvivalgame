#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <cassert>

using std::vector;
using std::wstring;
using std::string;

namespace utils
{
	string WStrToStr(const wstring & str);
	wstring StrToWStr(const string & str);
	vector<wstring> CollectFileNames(const wstring & Path, const wstring & Extension = L"*");
	wstring ExtractExtension( const wstring & filename );
	wstring ExtractName( const wstring & filename );
	bool IsNumber(const std::string & str);
	bool IsNumber(const std::wstring & str);

	namespace filesystem
	{
		bool CreateDir(const std::wstring & directoryName);
		bool CheckDir(const std::wstring & directoryName);
		bool DeleteDir(const std::wstring & directoryName);
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
		: deltaX(0), deltaY(0), deltaZ(0), absoluteX(0), absoluteY(0), clientX(0), clientY(0) {}

	MouseInfo(long _deltaX, long _deltaY, long _deltaZ, unsigned long _absoluteX, unsigned long _absoluteY, long _clientX, long _clientY)
		: deltaX(_deltaX), deltaY(_deltaY), deltaZ(_deltaZ), absoluteX(_absoluteX), absoluteY(_absoluteY), clientX(_clientX), clientY(_clientY) {}
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
