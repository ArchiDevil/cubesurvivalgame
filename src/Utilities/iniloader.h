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

	bool			Initialize(std::string FileName);
	int				GetInteger(std::string Parameter);
	float			GetFloat(std::string Parameter);
	bool			GetBoolean(std::string Parameter);
	std::string		GetString(std::string Parameter);
	std::wstring	GetWString(std::string Parameter);

private:
	void			CollectKeys(std::ifstream & stream);
	std::string		GetKey(std::string Key);

	std::map<std::string, std::string> data;
};