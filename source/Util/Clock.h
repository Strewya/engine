#ifndef UTIL_CLOCK_H_
#define UTIL_CLOCK_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <chrono>
#include <vector>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	class ITimer;
	
	class Clock
	{
	private:
		std::vector<ITimer*> _timers;
		std::chrono::time_point<std::chrono::high_resolution_clock> _lastUpdate;
		float _maxDeltaAllowed;

		Clock();
		static Clock _wallClock;
	public:
		static Clock& WallClock();
		void AdvanceTime();
		void RegisterTimer(ITimer* timer);
		void UnregisterTimer(ITimer* timer);

		float getMaxDeltaAllowed() const;
		void  setMaxDeltaAllowed(float delta);
	};
}

#endif //UTIL_CLOCK_H_