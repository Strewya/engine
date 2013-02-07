#ifndef GRAPHICS_RENDERINGQUEUE_H_
#define GRAPHICS_RENDERINGQUEUE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <map>
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
		void Add(int layer, Core::Form* f);
		void Clear();
		void Render(Interface& gfx) const;

	private:
		typedef std::vector<Core::Form*> FormVector;
		typedef std::map<int, FormVector> FormMap;

		FormMap _queue;


	};
}

#endif //GRAPHICS_RENDERINGQUEUE_H_