#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Util/ITimer.h"
#include "Util/ITimerComposer.h"
	/*** end header inclusion ***/

namespace Util
{
	class CompositeTimer : public ITimer, public ITimerComposer
	{
	private:
		std::deque<ITimer*> _timers;

		unsigned int FindFirstFreeSlot();

	public:
		void AdvanceTime(float delta);

		void RegisterTimer(ITimer& timer);
		void UnregisterTimer(ITimer& timer);

	};
}