//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Util/Clock.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
	/*** end headers ***/

Util::Clock gClock;

namespace Util
{
	void Clock::FrameStep()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
		float delta = std::chrono::duration_cast<std::chrono::duration<float>>(now-_lastUpdate).count();
		delta = delta > _maxDeltaAllowed ? _maxDeltaAllowed : delta;
		for(auto timer : _timers)
		{
			if(timer) timer->FeedTimeDelta(delta);
		}
	}

	void Clock::RegisterTimer(Timer* timer)
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
			_timers[slot-_timers.begin()] = timer;
		}
	}

	void Clock::UnregisterTimer(Timer* timer)
	{
		auto it = std::find_if(_timers.begin(), _timers.end(), [&timer](const Timer* t){return timer == t;});
		if(it != _timers.end())
		{
			_timers[it-_timers.begin()] = nullptr;
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



	Timer::Timer()
		: _scale(1), _deltaTime(0), _accumulator(0)
	{
		gClock.RegisterTimer(this);
	}

	Timer::~Timer()
	{
		gClock.UnregisterTimer(this);
	}

	void Timer::FeedTimeDelta(float dt)
	{
		_deltaTime = dt;
		_accumulator += (dt*_scale);
	}

	float Timer::getLastTimeDelta() const
	{
		return _deltaTime;
	}

	void Timer::Reset()
	{
		_accumulator = 0.0f;
	}

	bool Timer::EatTime(float time)
	{
		if(_accumulator >= time)
		{
			_accumulator -= time;
			return true;
		}
		return false;
	}

	float Timer::getElapsedTime() const
	{
		return _accumulator;
	}

	void Timer::setTimeScaling(float scale)
	{
		_scale = scale;
	}

	float Timer::getTimeScaling() const
	{
		return _scale;
	}
}