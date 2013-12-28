#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class Entity;
	class ResourceLocator;
	class ServiceLocator;
}

namespace Graphics
{
	class IRenderer;

    class RenderingQueue
	{
	public:
		void Add(Core::Entity& f);
		void Add(Core::Entity* f);
		void Clear();
		void Render(const Core::ServiceLocator& services, const Core::ResourceLocator& resources) const;

	private:
		typedef std::deque<Core::Entity*> FormVector;

		FormVector _queue;
	};
}