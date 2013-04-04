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
		double _accumulator;
		double _scale;
		double _deltaTime;

	public:
		void AdvanceTime(float dt);
		void setTimeScaling(double scale);
		void Reset();

		bool TimeToUpdate(double time);

		double getLastTimeDelta() const;
		double getElapsedTime() const;
		double getTimeScaling() const;
	};
}

#endif //UTIL_TIMER_H_