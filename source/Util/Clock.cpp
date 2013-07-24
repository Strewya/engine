//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Util/Clock.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
#include "Util/ITimer.h"
	/*** end headers ***/

namespace Util
{
	Clock::Clock()
		: _maxDeltaAllowed(0.25), _lastUpdate(std::chrono::high_resolution_clock::now())
	{}

	void Clock::AdvanceTime()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::duration<float>>(now-_lastUpdate).count();
		_lastUpdate = now;
		delta = delta > _maxDeltaAllowed ? _maxDeltaAllowed : delta;
		for(auto timer : _timers)
		{
			if(timer)
			{
				timer->AdvanceTime(delta);
			}
		}
	}

	uint32_t Clock::FindFirstFreeSlot()
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

	void Clock::RegisterTimer(ITimer& timer)
	{
		timer.id = FindFirstFreeSlot();
		_timers[timer.id] = &timer;
		timer.composer = this;
	}

	void Clock::UnregisterTimer(ITimer& timer)
	{
		/*
		if(_timers.empty())
		{
			return;
		}*/

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

	float Clock::getMaxDeltaAllowed() const
	{
		return _maxDeltaAllowed;
	}

	void Clock::setMaxDeltaAllowed(float delta)
	{
		_maxDeltaAllowed = delta;
	}
}