//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Util/Clock.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
#include "Util/ITimer.h"
	/*** end headers ***/

namespace Util
{
	Clock Clock::_wallClock;
	Clock& ITimer::_wallClock = Clock::WallClock();

	Clock& Clock::WallClock()
	{
		return _wallClock;
	}

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
				timer->AdvanceTime(delta);
		}
	}

	void Clock::RegisterTimer(ITimer* timer)
	{
		auto slot = _timers.end();
		for(auto it = _timers.begin(); it != _timers.end(); ++it)
		{
			if(slot == _timers.end() && *it == nullptr)
			{
				slot = it;
			}
			if(*it == timer)
			{
				return;
			}
		}
		if(slot == _timers.end())
		{
			_timers.push_back(timer);
		}
		else
		{
			*slot = timer;
		}
	}

	void Clock::UnregisterTimer(ITimer* timer)
	{
		auto it = std::find_if(_timers.begin(), _timers.end(), [&timer](const ITimer* t){return timer == t;});
		if(it != _timers.end())
		{
			*it = nullptr;
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