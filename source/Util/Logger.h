#ifndef UTIL_LOGGER_H_
#define UTIL_LOGGER_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <fstream>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	class Logger
	{
	public:
		Logger();
		Logger(const String& file);
		Logger(const char* file);
		~Logger();

		static const char endl = '\n';

		void Open(const String& file);
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

#endif //UTIL_LOGGER_H_