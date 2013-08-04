//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Util/Logger.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Util
{
	const char Logger::endl = '\n';

	Logger::Logger()
	{
		Open("logger.log");
	}

	Logger::Logger(const String& file)
	{
		Open(file.c_str());
	}

	Logger::Logger(const char* file)
	{
		Open(file);
	}

	Logger::~Logger() { _output.flush(); }

	void Logger::Open(const String& file)
	{
		Open(file.c_str());
	}

	void Logger::Open(const char* file)
	{
		if(_output.is_open())
			_output.close();
		_output.open(file, std::ios_base::app);
	}

	Logger& GetDefaultLogger()
	{
		static Logger log;
		return log;
	}
}
