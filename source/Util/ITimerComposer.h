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
	class ITimer;

	class ITimerComposer
	{
	public:
		virtual ~ITimerComposer() {};
		virtual void RegisterTimer(ITimer& t) = 0;
		virtual void UnregisterTimer(ITimer& t) = 0;
	};
}
