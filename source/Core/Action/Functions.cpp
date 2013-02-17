//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/Functions.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Entity/Entity.h"
#include "Subsystems/Graphics/RenderingQueue.h"
	/*** end headers ***/

namespace Core
{
	void Render(const std::set<Core::Entity*>& ents)
	{
		Graphics::RenderingQueue alphaQueue;
		for(Entity* entity : ents)
		{
			alphaQueue.Add(entity->getForm());
		}


	}
}