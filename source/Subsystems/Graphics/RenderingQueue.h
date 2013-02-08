#ifndef GRAPHICS_RENDERINGQUEUE_H_
#define GRAPHICS_RENDERINGQUEUE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <vector>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class Form;
}

namespace Graphics
{
	class Interface;

    class RenderingQueue
	{
	public:
		void Add(Core::Form* f);
		void Clear();
		void Render(Interface& gfx) const;

	private:
		typedef std::vector<Core::Form*> FormVector;

		FormVector _queue;


	};
}

#endif //GRAPHICS_RENDERINGQUEUE_H_