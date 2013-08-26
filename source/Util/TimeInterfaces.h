#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <stdint.h>
#include <chrono>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	class IExternalTimeSource
	{
	public:
		virtual int64_t getCurrentTime() = 0;
	};

	class HighPrecisionTimeSource : public IExternalTimeSource
	{
	public:
		int64_t getCurrentTime()
		{
			auto now = std::chrono::high_resolution_clock::now();
			
		}
	};
}
