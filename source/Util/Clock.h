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
		float _accumulator;
		float _scale;
		float _deltaTime;

	public:
		Timer();
		~Timer();
		
		void AdvanceTime(float dt);
		void setTimeScaling(float scale);
		void Reset();

		bool TimeToUpdate(float time);

		float getLastTimeDelta() const;
		float getElapsedTime() const;
		float getTimeScaling() const;
	};

	class Clock
	{
	private:
		std::vector<Timer*> _timers;
		std::chrono::time_point<std::chrono::high_resolution_clock> _lastUpdate;
		float _maxDeltaAllowed;

	public:
		Clock();
		void FrameStep();
		void RegisterTimer(Timer* timer);
		void UnregisterTimer(Timer* timer);

		float getMaxDeltaAllowed() const;
		void  setMaxDeltaAllowed(float delta);
	};
}

extern Util::Clock gClock;

#endif //UTIL_CLOCK_H_