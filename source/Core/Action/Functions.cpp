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
		for(auto& action : actions)
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
		for(auto& action : actions)
		{
			action->getOwner().getForm().Rotate(1);
		}
	}

	void InfinityPattern(double dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		for(auto& action : actions)
		{
			Entity& entity = action->getOwner();
			if( entity.getStates().Contains("SpringVel") && 
				entity.getStates().Contains("SpringMaxVel") && 
				entity.getStates().Contains("SpringAcc") && 
				entity.getStates().Contains("SpringForce"))
			{
				Util::Vec2& vel = entity.getStates().GetValue<Util::Vec2>("SpringVel");
				Util::Vec2& maxvel = entity.getStates().GetValue<Util::Vec2>("SpringMaxVel");
				Util::Vec2& acc = entity.getStates().GetValue<Util::Vec2>("SpringAcc");
				Util::Vec2& force = entity.getStates().GetValue<Util::Vec2>("SpringForce");
				
				IState* pOrigin = entity.getStates().GetState("SpringOrigin");
				if(pOrigin == nullptr)
				{
					entity.getStates().AddState("SpringOrigin", IState::Create(entity.getForm().getPosition()));
					pOrigin = entity.getStates().GetState("SpringOrigin");
				}
				Util::Vec2 origin = pOrigin->as<Util::Vec2>();
				float mass = 1.0f;
				Util::Vec2 fmass = force/mass;

				if(entity.getForm().getPosition().x < origin.x && acc.x < 0)
				{
					acc.x = fmass.x;
				}
				if(entity.getForm().getPosition().y < origin.y && acc.y < 0)
				{
					acc.y = fmass.y;
				}
				if(entity.getForm().getPosition().x > origin.x && acc.x > 0)
				{
					acc.x = -fmass.x;
				}
				if(entity.getForm().getPosition().y > origin.y && acc.y > 0)
				{
					acc.y = -fmass.y;
				}

				vel += acc;
				if(vel.x > maxvel.x) vel.x = maxvel.x;
				if(vel.x < -maxvel.x) vel.x = -maxvel.x;
				if(vel.y > maxvel.y) vel.y = maxvel.y;
				if(vel.y < -maxvel.y) vel.y = -maxvel.y;
				entity.getForm().Translate(vel*dt);
				
			}
		}
	}
}