//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Util/CompositeTimer.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
#include "Engine/Defines.h"
	/*** end headers ***/

namespace Util
{
	void CompositeTimer::AdvanceTime(float delta)
	{
		if(isPaused)
		{
			return;
		}

		for(auto* timer : _timers)
		{
			if(timer)
			{
				timer->AdvanceTime(delta);
			}
		}
	}

	uint32_t CompositeTimer::FindFirstFreeSlot()
	{
		for(auto it = _timers.begin(); it != _timers.end(); ++it)
		{
			if(*it == nullptr)
			{
				return it - _timers.begin();
			}
		}
		_timers.emplace_back();
		return _timers.size()-1;
	}

	void CompositeTimer::RegisterTimer(ITimer& timer)
	{
		timer.id = FindFirstFreeSlot();
		_timers[timer.id] = &timer;
		timer.composer = this;
	}

	void CompositeTimer::UnregisterTimer(ITimer& timer)
	{
		if(_timers[timer.id] == &timer)
		{
			_timers[timer.id] = nullptr;
			timer.id = NOT_FOUND;
			timer.composer = nullptr;
		}
		else
		{
			auto it = std::find_if(_timers.begin(), _timers.end(), [&timer](const ITimer* t){return &timer == t;});
			if(it != _timers.end())
			{
				*it = nullptr;
				timer.id = NOT_FOUND;
				timer.composer = nullptr;
			}
		}
	}
}