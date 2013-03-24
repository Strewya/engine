#ifndef SUBSYSTEMS_GRAPHICS_RENDERINGQUEUE_H_
#define SUBSYSTEMS_GRAPHICS_RENDERINGQUEUE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class Form;
	class ResourceLocator;
	class ServiceLocator;
}

namespace Graphics
{
	class IRenderer;

    class RenderingQueue
	{
	public:
		void Add(Core::Form& f);
		void Add(Core::Form* f);
		void Clear();
		void Render(const Core::ServiceLocator& services, const Core::ResourceLocator& resources) const;

	private:
		typedef std::deque<Core::Form*> FormVector;

		FormVector _queue;
	};
}

#endif //SUBSYSTEMS_GRAPHICS_RENDERINGQUEUE_H_