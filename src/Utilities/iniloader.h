#pragma once

#include <fstream>
#include <string>
#include <map>

#include "ut.h"

class IniWorker
{
public:
	IniWorker();
	~IniWorker();

	bool			Initialize(const std::string & FileName);
	int				GetInteger(const std::string & Parameter);
	float			GetFloat(const std::string & Parameter);
	bool			GetBoolean(const std::string & Parameter);
	std::string		GetString(const std::string & Parameter);
	std::wstring	GetWString(const std::string & Parameter);

private:
	void			CollectKeys(std::ifstream & stream);
	std::string		GetKey(const std::string & Key);

	std::map<std::string, std::string> data;
};