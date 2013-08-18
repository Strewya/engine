//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Util/Timer.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/Defines.h"
	/*** end headers ***/

namespace Util
{
	Timer::Timer()
		: _accumulator(0), _scale(1), _updatePeriod(gUpdateInterval)
	{}

	Timer::Timer(float updatePeriod, float scale)
		: _accumulator(0), _scale(scale), _updatePeriod(updatePeriod)
	{}

	void Timer::AdvanceTime(float dt)
	{
		if(isPaused)
		{
			return;
		}

		_accumulator += (dt*_scale);
	}

	float Timer::getUpdatePeriod() const
	{
		return _updatePeriod;
	}

	void Timer::setUpdatePeriod(float dt)
	{
		_updatePeriod = dt;
	}

	void Timer::Reset()
	{
		_accumulator = 0.0;
	}

	bool Timer::HasUpdatePeriodElapsed()
	{
		if(_accumulator >= _updatePeriod)
		{
			_accumulator -= _updatePeriod;
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