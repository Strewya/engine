//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Util/ITimer.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Util/Clock.h"
	/*** end headers ***/

namespace Util
{
	ITimer::ITimer()
	{
		_wallClock.RegisterTimer(this);
	}

	ITimer::ITimer(const ITimer& rhs)
	{
		_wallClock.RegisterTimer(this);
	}

	ITimer::~ITimer()
	{
		_wallClock.UnregisterTimer(this);
	}
}