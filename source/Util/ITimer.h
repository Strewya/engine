#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <stdint.h>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	class ITimerComposer;

	class ITimer
	{
	public:
		uint32_t id;
		ITimerComposer* composer;
		bool isPaused;

		ITimer();
		ITimer(const ITimer& rhs);
		ITimer& operator=(const ITimer& rhs);
		virtual ~ITimer();

		virtual void AdvanceTime(float dt) = 0;
	};
}
