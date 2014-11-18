//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/RenderQueue.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	bool RenderQueue::init(GraphicsSystem& graphics)
	{
		DEBUG_STATUS(true);

		m_graphicsSystem = &graphics;

		DEBUG_INIT(RenderQueue);
	}

	bool RenderQueue::shutdown()
	{
		DEBUG_STATUS(true);

		DEBUG_SHUTDOWN(RenderQueue);
	}


}
