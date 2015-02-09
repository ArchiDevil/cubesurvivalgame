#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <tuple>
#include <sstream>

#if defined (WIN32) || (_WIN32)
    #include <Windows.h>
#endif

class Log
{
public:
    Log( const std::string & FileName )
    {
        log.open(FileName);
    }

    ~Log()
    {
        log.close();
    }

    template<typename ... Args>
	void Message(const Args & ... args)
    {
#if defined (DEBUG) || (_DEBUG)
        log << "INFO: ";
        std::cout << "INFO: ";
        log_message(args...);
#else
        empty(args...);
#endif
    }
    
    template<typename ... Args>
	void Error(const Args & ... args)
    {
        log << "ERROR: ";
        std::cout << "ERROR: ";
        log_message(args...);
    }
    
    template<typename ... Args>
	void FatalError(const Args & ... args)
    {
        log << "FATAL ERROR: ";
        std::cout << "FATAL ERROR: ";
        log_message(args...);

		std::stringstream s;
		to_stringstream(s, args...);

#if defined (WIN32) || (_WIN32)
		::MessageBoxA(NULL, s.str().c_str(), "Fatal error!", MB_ICONERROR);
#endif
		throw std::runtime_error(s.str().c_str());
    }

private:
    template<typename T>
    void empty(const T&) {}

    template<typename T, typename ... Args>
    void empty(const T&, const Args & ... args)
    {
        empty(args...);
    }

    template<typename T>
    void log_message(const T & arg)
    {
    	log << arg << std::endl;
        std::cout << arg << std::endl;
    }

    template<typename T, typename ... Args>
    void log_message(const T & arg, const Args & ... args)
    {
        log << arg;
        std::cout << arg;
        log_message(args...);
    }

	template<typename T>
	void to_stringstream(std::stringstream & s)
	{
		return;
	}

	template<typename T>
	void to_stringstream(std::stringstream & s, const T& arg) const
	{
		s << arg;
	}

	template<typename T, typename ... Args>
	void to_stringstream(std::stringstream & s, const T& arg, const Args & ... args) const
	{
		s << arg;
		to_stringstream(s, args...);
	}

	std::ofstream log;

};

extern Log MainLog;

#define LOG_FATAL_ERROR(...)			MainLog.FatalError(__FUNCTION__, "():", __LINE__, " -- ", __VA_ARGS__)
#define LOG_ERROR(...)					MainLog.Error(__FUNCTION__, "():", __LINE__, " -- ", __VA_ARGS__)
#define LOG_INFO(...)					MainLog.Message(__FUNCTION__, "():", __LINE__, " -- ", __VA_ARGS__)
