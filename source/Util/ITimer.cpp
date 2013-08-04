//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Util/ITimer.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Util/Clock.h"
	/*** end headers ***/

namespace Util
{
	ITimer::ITimer()
		: id((uint32_t)NOT_FOUND), composer(nullptr), isPaused(false)
	{}

	ITimer::ITimer(const ITimer& rhs)
		: id((uint32_t)NOT_FOUND), composer(nullptr), isPaused(false)
	{
	}

	ITimer& ITimer::operator=(const ITimer& rhs)
	{
		return *this;
	}

	ITimer::~ITimer()
	{
		if(composer != nullptr && id != (uint32_t)NOT_FOUND)
		{
			composer->UnregisterTimer(*this);
		}
	}
}
