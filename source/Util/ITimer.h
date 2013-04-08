#ifndef UTIL_ITIMER_H_
#define UTIL_ITIMER_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	class Clock;

	class ITimer
	{
	private:
		static Clock& _wallClock;
	public:
		ITimer();
		ITimer(const ITimer& rhs);
		virtual ~ITimer();
		virtual void AdvanceTime(float dt) = 0;
	};
}

#endif //UTIL_ITIMER_H_