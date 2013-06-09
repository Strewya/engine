#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
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