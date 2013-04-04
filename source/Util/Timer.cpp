//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Util/Timer.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Util
{
	void Timer::AdvanceTime(float dt)
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