#pragma once

#include <string>
#include <fstream>

//#define LOG_FATAL_ERROR(fmt, ...)		MainLog.FatalError(__FILE__##":"##__LINE__##" "##fmt, ##__VA_ARGS__)
//#define LOG_ERROR(fmt, ...)			MainLog.Error(__FILE__##":"##__LINE__##" "##fmt, ##__VA_ARGS__)
//#define LOG_INFO(fmt, ...)			MainLog.Message(__FILE__##":"##__LINE__##" "##fmt, ##__VA_ARGS__)

class Log
{
public:
	Log(const std::string & FileName);
	~Log();

	void Message(const std::string & message );
	void Error(const std::string & error );
	void FatalError(const std::string & error );

private:
	static std::string GetTimeString();
	std::ofstream log;

};

extern Log MainLog;
