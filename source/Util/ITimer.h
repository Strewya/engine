#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	class ITimerComposer;

	class ITimer
	{
	public:
		unsigned int id;
		ITimerComposer* composer;
		bool isPaused;

		ITimer();
		ITimer(const ITimer& rhs);
		ITimer& operator=(const ITimer& rhs);
		virtual ~ITimer();

		virtual void AdvanceTime(float dt) = 0;
	};
}