#include "logger.h"

#if defined (WIN32) || (_WIN32)
    #include <Windows.h>
#endif

#include <sstream>
#include <cstdlib>
#include <iostream>

Log MainLog("Application.log");

Log::Log( const std::string & FileName )
{
	log.open(FileName);
}

Log::~Log()
{
	log.close();
}

void Log::Message( const std::string & message )
{
#if defined (DEBUG) || (_DEBUG)
	log << GetTimeString() << "INFO: " << message << std::endl;
	std::cout << GetTimeString() << "INFO: " << message << std::endl;
#endif // DEBUG
}

void Log::Error( const std::string & error )
{
	log << GetTimeString() << "ERROR: " << error << std::endl;
	std::cout << GetTimeString() << "ERROR: " << error << std::endl;
}

void Log::FatalError( const std::string & error )
{
	log << GetTimeString() << "FATAL ERROR: " << error << std::endl;
	std::cout << GetTimeString() << "FATAL ERROR: " << error << std::endl;

#if defined (WIN32) || (_WIN32)
	::MessageBoxA(NULL, error.c_str(), "Fatal error!", MB_ICONERROR);
#endif
	throw std::runtime_error(error);
}

std::string Log::GetTimeString()
{
#if defined (WIN32) || (_WIN32)
	SYSTEMTIME sm;
	GetLocalTime(&sm);
	std::ostringstream buffer;
	buffer << "[["
		<< sm.wDay
		<< "."
		<< sm.wMonth
		<< "."
		<< sm.wYear
		<< "] "
		<< sm.wHour
		<< ":"
		<< sm.wMinute
		<< ":"
		<< sm.wSecond
		<< "]\t";
	return buffer.str();
#else
    tm * time;
    timelocal(time);
	std::ostringstream buffer;
	buffer << "[["
		<< time->tm_mday
		<< "."
		<< time->tm_mon
		<< "."
		<< time->tm_year
		<< "] "
		<< time->tm_hour
		<< ":"
		<< time->tm_min
		<< ":"
		<< time->tm_sec
		<< "]\t";
	return buffer.str();
#endif
}
