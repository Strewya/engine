//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/State/State4.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	BaseState::BaseState(InstanceID uid)
		: uid(uid)
	{}

	BaseState::~BaseState()
	{}
}