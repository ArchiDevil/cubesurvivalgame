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
	log << "INFO: " << message << std::endl;
	std::cout << "INFO: " << message << std::endl;
#endif // DEBUG
}

void Log::Error( const std::string & error )
{
	log << "ERROR: " << error << std::endl;
	std::cout << "ERROR: " << error << std::endl;
}

void Log::FatalError( const std::string & error )
{
	log << "FATAL ERROR: " << error << std::endl;
	std::cout << "FATAL ERROR: " << error << std::endl;

#if defined (WIN32) || (_WIN32)
	::MessageBoxA(NULL, error.c_str(), "Fatal error!", MB_ICONERROR);
#endif
	throw std::runtime_error(error);
}
