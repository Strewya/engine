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

	void Rotate(float dt, GameContext& context, std::set<Core::Entity*>& ents)
	{
		for(auto& entity : ents)
		{
			entity->getForm().Rotate(1);
		}
	}

	void InfinityPattern(float dt, GameContext& context, std::set<Core::Entity*>& ents)
	{
		for(auto& entity : ents)
		{
			if( entity->getStates().Contains("SpringVel") && 
				entity->getStates().Contains("SpringAcc") && 
				entity->getStates().Contains("SpringForce"))
			{
				Util::Vec2& vel = entity->getStates().GetValue<Util::Vec2>("SpringVel");
				Util::Vec2& acc = entity->getStates().GetValue<Util::Vec2>("SpringAcc");
				Util::Vec2& force = entity->getStates().GetValue<Util::Vec2>("SpringForce");
				float mass = 1;

				acc = force/mass;
				vel = acc*dt;
				entity->getForm().Translate(vel);
			}
		}
	}
}