//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/Functions.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "GameContext.h"
#include "Core/Action/Action.h"
#include "Core/Entity/Entity.h"
#include "Subsystems/Graphics/IRenderer.h"
#include "Subsystems/Graphics/RenderingQueue.h"
#include "Subsystems/Input/InputEngine.h"
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
		
		alphaQueue.Render(context.services, context.resources);

		context.services.getGraphics().EndSpriteBatch();
		context.services.getGraphics().EndScene();
	}

	void Animate(double dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		for(auto action : actions)
		{
			auto& owner = action->getOwner().getForm();
			if(owner.hasState("CurrentSprite") && owner.hasState("CurrentAnimationFrame") && owner.hasState("CurrentAnimation") && owner.hasState("Spritesheet"))
			{
				IState* states[] = {owner.getState("Spritesheet"), owner.getState("CurrentAnimation"), owner.getState("CurrentAnimationFrame"), owner.getState("CurrentSprite")};
				auto& spritesheet = context.resources.getSpritesheetCache().getSpritesheet(states[0]->as<String>());
				auto& animation = spritesheet.getAnimation(states[1]->as<uint>());
				auto& curFrame = states[2]->as<uint>();
				auto& curSprite = states[3]->as<uint>();
				curFrame = (++curFrame) % animation.getSequenceSize();
				curSprite = animation.getSequenceFrame(curFrame);
			}
		}
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

	void SIH(double dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		for(auto action : actions)
		{
			auto& entity = action->getOwner();
			if(context.services.getInput().isKeyPressed(Input::Keyboard::_ArrowLeft))
			{
				entity.getForm().Translate(-2, 0);
			}
			if(context.services.getInput().isKeyPressed(Input::Keyboard::_ArrowRight))
			{
				entity.getForm().Translate(2, 0);
			}
		}
	}
}