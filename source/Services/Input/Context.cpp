//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/Context.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Services/input/Event.h"
	/*** end headers ***/

namespace Input
{
	
	Context::Context()
	{
	}

	uint32_t Context::tryMapping(Event& e) const
	{
		return 0;
	}

	void Context::addMapping(uint32_t intent, std::function<bool(Event&)> trigger)
	{

	}
}