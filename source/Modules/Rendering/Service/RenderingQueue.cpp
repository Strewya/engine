//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Modules/Rendering/Service/RenderingQueue.h>
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	void RenderingQueue::Add(Core::Entity& f)
    {
		_queue.push_back(&f);
    }

	void RenderingQueue::Add(Core::Entity* f)
    {
		_queue.push_back(f);
    }
    
    void RenderingQueue::Clear()
    {
		_queue.clear();
    }
    
	void RenderingQueue::Render(const Core::ServiceLocator& services, const Core::ResourceLocator& resources) const
	{
        //(void) services;
        //(void) resources;
	}
}
