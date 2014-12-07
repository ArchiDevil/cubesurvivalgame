#pragma once

#include <string>
#include <fstream>

class Log
{
public:
	Log(const std::string & FileName);
	~Log();

	void Message(const std::string & message);
	void Error(const std::string & error);
	void FatalError(const std::string & error);

private:
	std::ofstream log;

};

extern Log MainLog;

#define LOG_FATAL_ERROR(str)			MainLog.FatalError(std::string(__FUNCTION__) + "():" + std::to_string(__LINE__) + " " + str)
#define LOG_ERROR(str)					MainLog.Error(std::string(__FUNCTION__) + "():" + std::to_string(__LINE__) + " " + str)
#define LOG_INFO(str)					MainLog.Message(std::string(__FUNCTION__) + "():" + std::to_string(__LINE__) + " " + str)
