//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/RenderingQueue.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Form/Form.h"
#include "Subsystems/Graphics/Interface.h"
	/*** end headers ***/

namespace Graphics
{
	void RenderingQueue::Add(int layer, Core::Form* f)
    {
		FormVector& fset = _queue[layer];
		fset.push_back(f);
    }
    
    void RenderingQueue::Clear()
    {
		_queue.clear();
    }
    
	void RenderingQueue::Render(Interface& gfx) const
	{
		for( auto& vec : _queue)
		{
			Core::DrawForms(vec.second, gfx);
		}
	}
}