#include "cRegWorker.h"

cRegWorker::cRegWorker()
{}

cRegWorker::~cRegWorker()
{}

std::wstring cRegWorker::GetString( Root root, std::wstring path, std::wstring parameter )
{
	HKEY hKey;
	wchar_t buffer[1024] = {0};
	DWORD a = 1024;
	DWORD typ;
	RegOpenKeyEx( (HKEY)(ULONG_PTR)root, path.c_str(), 0, KEY_READ, &hKey);
	RegQueryValueEx(hKey, parameter.c_str(), NULL, &typ, (LPBYTE)buffer, &a);
	RegCloseKey(hKey);
	std::wstring out = buffer;
	return out;
}

int cRegWorker::GetInteger( Root root, std::wstring path, std::wstring parameter )
{
	HKEY hKey;
	DWORD out = 0;
	DWORD a = sizeof(DWORD);
	RegOpenKeyEx( (HKEY)(ULONG_PTR)root, path.c_str(), 0, KEY_READ, &hKey);
	RegQueryValueEx(hKey, parameter.c_str(), NULL, NULL, (LPBYTE)&out, &a);
	RegCloseKey(hKey);
	return out;
}
