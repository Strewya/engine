#ifndef UTIL_TIMER_H_
#define UTIL_TIMER_H_
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

		bool HasUpdatePeriodElapsed();

		float getElapsedTime() const;
		float getTimeScaling() const;
		float getUpdatePeriod() const;
	};
}

#endif //UTIL_TIMER_H_