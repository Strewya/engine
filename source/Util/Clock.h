#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <chrono>
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Util/ITimerComposer.h"
	/*** end header inclusion ***/

namespace Util
{
	class ITimer;
	
	class Clock : public ITimerComposer
	{
	private:
		std::deque<ITimer*> _timers;
		std::chrono::time_point<std::chrono::high_resolution_clock> _lastUpdate;
		float _maxDeltaAllowed;

		uint FindFirstFreeSlot();
	public:
		Clock();
		void AdvanceTime();
		void RegisterTimer(ITimer& timer);
		void UnregisterTimer(ITimer& timer);

		float getMaxDeltaAllowed() const;
		void  setMaxDeltaAllowed(float delta);
	};
}