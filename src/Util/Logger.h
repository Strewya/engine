#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <fstream>
#include <string>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class Logger
	{
	public:
		Logger();
		Logger(const std::string& file);
		Logger(const char* file);
		~Logger();

		static const char endl;

		void Open(const std::string& file);
		void Open(const char* file);
		
		template <class T>
		Logger& operator<<(const T& t)
		{
			_output << t; _output.flush(); return *this;
		}
		
	private:
		Logger(Logger& l);
		Logger& operator=(const Logger &log);

		std::ofstream _output;
	};

	Logger& GetDefaultLogger();

	template <class T>
	void Log(const T& value)
	{
		Logger& logger = GetDefaultLogger();
		logger << value;
	}

	template <class T>
	void LogLine(const T& value)
	{
		Logger& logger = GetDefaultLogger();
		logger << value << Logger::endl;
	}
}