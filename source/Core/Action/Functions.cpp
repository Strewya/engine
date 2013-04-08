//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Action/Functions.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Engine/Engine.h"
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
		State* s_currentSprite = nullptr;
		State* s_animationFrame = nullptr;
		State* s_animation = nullptr;
		State* s_spritesheet = nullptr;
		State* s_timer = nullptr;

		for(auto action : actions)
		{
			auto& owner = action->getOwner().getForm();
			s_spritesheet = owner.getState("Spritesheet");
			s_animation = owner.getState("CurrentAnimation");
			s_animationFrame = owner.getState("CurrentAnimationFrame");
			s_currentSprite = owner.getState("CurrentSprite");
			s_timer = owner.getState("AnimationTimer");
			if(s_currentSprite && s_animation && s_animationFrame && s_spritesheet)
			{
				if(s_timer && !s_timer->as<Util::Timer>().HasUpdatePeriodElapsed())
				{
					continue;
				}
				auto& spritesheet = s_spritesheet->as<Graphics::Spritesheet&>();
				auto& animation = spritesheet.getAnimation(s_animation->as<uint>());
				auto& curFrame = s_animationFrame->as<uint>();
				auto& curSprite = s_currentSprite->as<uint>();
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
		
	}

	void SIH(double dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		float speed = 5;
		for(auto action : actions)
		{
			auto& entity = action->getOwner();
			if(context.services.getInput().isKeyPressed(Input::Keyboard::_ArrowLeft))
			{
				entity.getForm().Translate(-speed, 0);
			}
			if(context.services.getInput().isKeyPressed(Input::Keyboard::_ArrowRight))
			{
				entity.getForm().Translate(speed, 0);
			}
			if(context.services.getInput().isKeyPressed(Input::Keyboard::_ArrowDown))
			{
				entity.getForm().Translate(0, speed);
			}
			if(context.services.getInput().isKeyPressed(Input::Keyboard::_ArrowUp))
			{
				entity.getForm().Translate(0, -speed);
			}
		}

		Input::Event event;
		while(context.services.getInput().PollEvent(event))
		{
			for(auto action : actions)
			{
				switch(event.type)
				{
				case Input::EventType::KeyPressed:
					{
						if(event.key.code == Input::Keyboard::_Space)
						{
							action->getOwner().getForm().setVisibility(!action->getOwner().getForm().getVisibility());
						}
					}
				}
			}
		}

		if(context.services.getInput().isKeyPressed(Input::Keyboard::_Escape))
		{
			context.services.getEngine().Shutdown();
		}
	}
}