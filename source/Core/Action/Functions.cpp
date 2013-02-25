//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/Functions.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "ServiceLocator.h"
#include "Core/Entity/Entity.h"
#include "Subsystems/Graphics/Interface.h"
#include "Subsystems/Graphics/RenderingQueue.h"
	/*** end headers ***/

namespace Core
{
	void Render(float dt, /*const ResourceLocator& resources,*/ const Core::ServiceLocator& services, std::set<Core::Entity*>& ents)
	{
		Graphics::RenderingQueue alphaQueue;
		for(auto& entity : ents)
		{
			alphaQueue.Add(entity->getForm());
		}

		services.getGraphics().BeginScene();
		services.getGraphics().BeginSpriteBatch(true);
		
		alphaQueue.Render(services.getGraphics());

		services.getGraphics().EndSpriteBatch();
		services.getGraphics().EndScene();
	}
}