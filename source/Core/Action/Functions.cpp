//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/Functions.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "GameContext.h"
#include "Core/Action/Action.h"
#include "Core/Entity/Entity.h"
#include "Subsystems/Graphics/Interface.h"
#include "Subsystems/Graphics/RenderingQueue.h"
	/*** end headers ***/

namespace Core
{
	void Render(double dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		Graphics::RenderingQueue alphaQueue;
		for(auto action : actions)
		{
			alphaQueue.Add(action->getOwner().getForm());
		}

		context.services.getGraphics().BeginScene();
		context.services.getGraphics().BeginSpriteBatch(true);
		
		alphaQueue.Render(context.services.getGraphics());

		context.services.getGraphics().EndSpriteBatch();
		context.services.getGraphics().EndScene();
	}

	void Rotate(double dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		for(auto action : actions)
		{
			action->getOwner().getForm().Rotate(1);
		}
	}

	void Spawner(double dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		for(auto action : actions)
		{
			auto spawnItem = action->getOwner().getState("SpawnItem");
			if(spawnItem)
			{
				String item = spawnItem->as<String>();
				
			}
		}
	}
}