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
	Clock::Clock()
		: _maxDeltaAllowed(0.25), _lastUpdate(std::chrono::high_resolution_clock::now())
	{}

	void Clock::AdvanceTime()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
		double delta = std::chrono::duration_cast<std::chrono::duration<double>>(now-_lastUpdate).count();
		_lastUpdate = now;
		delta = delta > _maxDeltaAllowed ? _maxDeltaAllowed : delta;
		for(auto timer : _timers)
		{
			if(timer) timer->AdvanceTime(delta);
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
			*slot = timer;
		}
	}

	void Clock::UnregisterTimer(Timer* timer)
	{
		auto it = std::find_if(_timers.begin(), _timers.end(), [&timer](const Timer* t){return timer == t;});
		if(it != _timers.end())
		{
			*it = nullptr;
		}
	}

	double Clock::getMaxDeltaAllowed() const
	{
		return _maxDeltaAllowed;
	}

	void Clock::setMaxDeltaAllowed(double delta)
	{
		_maxDeltaAllowed = delta;
	}



	Timer::Timer()
		: _scale(1), _deltaTime(0), _accumulator(0)
	{
		gClock.RegisterTimer(this);
	}

	Timer::Timer(const Timer& rhs)
		: _scale(rhs._scale), _deltaTime(rhs._deltaTime), _accumulator(rhs._accumulator)
	{
		gClock.RegisterTimer(this);
	}

	Timer& Timer::operator=(const Timer& rhs)
	{
		if(this != &rhs)
		{
			_scale = rhs._scale;
			_accumulator = rhs._accumulator;
			_deltaTime = rhs._deltaTime;
			gClock.RegisterTimer(this);
		}
		return *this;
	}

	Timer::~Timer()
	{
		gClock.UnregisterTimer(this);
	}

	void Timer::AdvanceTime(double dt)
	{
		_deltaTime = dt;
		_accumulator += (dt*_scale);
	}

	double Timer::getLastTimeDelta() const
	{
		return _deltaTime;
	}

	void Timer::Reset()
	{
		_accumulator = 0.0;
	}

	bool Timer::TimeToUpdate(double time)
	{
		if(_accumulator >= time)
		{
			_accumulator -= time;
			return true;
		}
		return false;
	}

	double Timer::getElapsedTime() const
	{
		return _accumulator;
	}

	void Timer::setTimeScaling(double scale)
	{
		_scale = scale;
	}

	double Timer::getTimeScaling() const
	{
		return _scale;
	}
}