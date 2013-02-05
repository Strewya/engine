	/*** personal header ***/
#include "SGRenderingQueue.h"
	/*** extra headers ***/
#include "SGForm.h"
	/*** end headers ***/

namespace SG
{
	bool FormPtrCompare::operator()(const Form* a, const Form* b) const
	{
		Vector apos = a->getPosition();
		Vector bpos = b->getPosition();

		if(apos.z < bpos.z)
			return true;
		else if(apos.z == bpos.z)
		{
			if(apos.y < bpos.y)
				return true;
			else if(apos.y == bpos.y)
			{
				if(apos.x < bpos.x)
					return true;
			}
		}
		return false;
	}

	void RenderingQueue::Add(Form* f)
    {
		FormSet& fset = _queue[f->getRenderPriority()];
		fset.insert(f);
    }
    
    void RenderingQueue::Remove(Form* f)
    {
		_queue[f->getRenderPriority()].erase(f);
    }
    
	void RenderingQueue::Render()
	{
		FormMap::iterator mapit = _queue.begin();
		FormSet::iterator setit;
		for(; mapit != _queue.end(); ++mapit)
		{
			FormSet& s = mapit->second;
			for( setit = s.begin(); setit != s.end(); ++setit)
			{
				(*setit)->DrawSelf();
			}
		}
	}
}