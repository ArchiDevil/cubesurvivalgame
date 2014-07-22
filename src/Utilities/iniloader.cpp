#include "iniloader.h"
#include "logger.h"

IniWorker::IniWorker()
{
}

IniWorker::~IniWorker()
{
}

bool IniWorker::Initialize( std::string FileName )
{
	std::ifstream File;
	File.open(FileName.c_str());
	if(File.fail())
	{
		MainLog.FatalError("Unable to open settings file");
		return false;
	}
	CollectKeys(File);
	return true;
}

std::string IniWorker::GetKey( std::string Key )
{
	if(data.find(Key) != data.end())
		return data[Key];
	else
		MainLog.Error("Unable to get key for: " + Key);
}

int IniWorker::GetInteger( std::string Parameter )
{
	return std::stoi(GetKey(Parameter));
}

float IniWorker::GetFloat( std::string Parameter )
{
	return std::stof(GetKey(Parameter));
}

bool IniWorker::GetBoolean( std::string Parameter )
{
	std::string buffer = GetKey(Parameter);
	if(buffer == "true")
		return true;
	else if(buffer == "false")
		return false;
	else
	{
		MainLog.Error(buffer + " is a wrong boolean value");
		return false;
	}
}

std::string IniWorker::GetString( std::string Parameter )
{
	return GetKey(Parameter);
}

std::wstring IniWorker::GetWString( std::string Parameter )
{
	return utils::StrToWStr(GetKey(Parameter));
}

void IniWorker::CollectKeys( std::ifstream & stream )
{
	std::string buffer = "";
	while (std::getline(stream, buffer))
	{
		std::string::size_type it = buffer.find('=');
		if(it != std::string::npos)
		{
			data[std::string(buffer.begin(), buffer.begin() + it)] = std::string(buffer.begin() + it + 1, buffer.end());
		}
	}
}
