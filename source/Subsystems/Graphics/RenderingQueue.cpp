//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/RenderingQueue.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Form/Form.h"
	/*** end headers ***/

namespace Graphics
{
	void RenderingQueue::Add(Core::Form* f)
    {
		_queue.push_back(f);
    }
    
    void RenderingQueue::Clear()
    {
		_queue.clear();
    }
    
	void RenderingQueue::Render(Interface& gfx) const
	{
		Core::DrawForms(_queue, gfx);
	}
}