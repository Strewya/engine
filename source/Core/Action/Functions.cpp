//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/Functions.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "GameContext.h"
#include "Core/Entity/Entity.h"
#include "Subsystems/Graphics/Interface.h"
#include "Subsystems/Graphics/RenderingQueue.h"
	/*** end headers ***/

namespace Core
{
	void Render(float dt, GameContext& context, std::set<Core::Entity*>& ents)
	{
		Graphics::RenderingQueue alphaQueue;
		for(auto& entity : ents)
		{
			alphaQueue.Add(entity->getForm());
		}

		context.services.getGraphics().BeginScene();
		context.services.getGraphics().BeginSpriteBatch(true);
		
		alphaQueue.Render(context.services.getGraphics());

		context.services.getGraphics().EndSpriteBatch();
		context.services.getGraphics().EndScene();
	}
}