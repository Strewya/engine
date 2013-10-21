//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Impl/Render.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Entity/Entity.h"
#include "Engine/GameContext.h"
#include "Engine/ServiceLocator.h"
#include "Services/Graphics/IRenderer.h"
	/*** end headers ***/

namespace Core
{
	void Render::frameUpdate(float dt)
	{
		for(auto id : m_entities)
		{
			//check if entity still exists, if not add it to deletion list
			if(m_context.entityPool.isAlive(id))
			{
			//[exists]
			//get entity
				//Entity* ent = context.entityPool.getInstancePtr(id);
			//get required components:

			//	RenderData
			//	Position
			//	..
			//check if entity should be drawn

			//[is drawn]
			//add entity to _queue
			}
		}
		//remove entities that are no longer valid from the list

		m_context.services.getGraphics().BeginScene();
		
		//_queue.Render(context.services, context.resources);
		
		m_context.physicsWorld.DrawDebugData();

		m_context.services.getGraphics().EndScene();
	}

	bool Render::validateEntity(Entity& entity) const
	{
		return true; // change to check if entity has renderable component
	}



	/*
	
	void Animate(float dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		StateRptr s_currentSprite = nullptr;
		StateRptr s_animationFrame = nullptr;
		StateRptr s_animation = nullptr;
		StateRptr s_spritesheet = nullptr;
		StateRptr s_timer = nullptr;

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
				uint32_t hSheet = s_spritesheet->as<uint32_t>();
				auto* spritesheet = context.resources.getSpritesheetCache().getSpritesheet(hSheet);
				auto& animation = spritesheet->getAnimation(s_animation->as<uint32_t>());
				auto& curFrame = s_animationFrame->as<uint32_t>();
				auto& curSprite = s_currentSprite->as<uint32_t>();
				curFrame = (++curFrame) % animation.getSequenceSize();
				curSprite = animation.getSequenceFrame(curFrame);
			}
		}
	}
	*/
}
