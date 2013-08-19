#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Util/ITimer.h"
	/*** end header inclusion ***/

namespace Util
{
	class Timer : public ITimer
	{
	private:
		float _accumulator;
		float _scale;
		float _updatePeriod;

	public:
		Timer();
		Timer(float updatePeriod, float scale = 1);

		void AdvanceTime(float dt);
		void setTimeScaling(float scale);
		void setUpdatePeriod(float updatePeriod);
		void Reset();

		bool hasUpdatePeriodElapsed();

		float getElapsedTime() const;
		float getTimeScaling() const;
		float getUpdatePeriod() const;
	};
}