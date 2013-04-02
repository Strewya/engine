#ifndef UTIL_CLOCK_H_
#define UTIL_CLOCK_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <chrono>
#include <vector>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	class Timer
	{
	private:
		double _accumulator;
		double _scale;
		double _deltaTime;

	public:
		Timer();
		Timer(const Timer& rhs);
		Timer& operator=(const Timer& rhs);
		~Timer();
		
		void AdvanceTime(double dt);
		void setTimeScaling(double scale);
		void Reset();

		bool TimeToUpdate(double time);

		double getLastTimeDelta() const;
		double getElapsedTime() const;
		double getTimeScaling() const;
	};

	class Clock
	{
	private:
		std::vector<Timer*> _timers;
		std::chrono::time_point<std::chrono::high_resolution_clock> _lastUpdate;
		double _maxDeltaAllowed;

	public:
		Clock();
		void AdvanceTime();
		void RegisterTimer(Timer* timer);
		void UnregisterTimer(Timer* timer);

		double getMaxDeltaAllowed() const;
		void  setMaxDeltaAllowed(double delta);
	};
}

extern Util::Clock gClock;

#endif //UTIL_CLOCK_H_