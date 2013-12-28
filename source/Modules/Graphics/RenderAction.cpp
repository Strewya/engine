//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Modules/Graphics/RenderAction.h>
	/*** C++ headers ***/
	/*** extra headers ***/
#include <Core/Entity/Entity.h>
#include <Engine/GameContext.h>
#include <Engine/ServiceLocator.h>
#include <Modules/Graphics/Service/IRenderer.h>
	/*** end headers ***/

namespace Core
{
	void Render::processMessage(GameContext& context, uint32_t msg, InstanceID entity)
	{
		
	}

	void Render::update(GameContext& context)
	{}

	void Render::render(GameContext& context, uint64_t interpolationTime)
	{
		for(auto id : m_entities)
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
		//remove entities that are no longer valid from the list

		//_queue.Render(context.services, context.resources);
	}




	void MainRender::update(GameContext& context)
	{}

	void MainRender::render(GameContext& context, uint64_t interpolationTime)
	{
		context.m_services.getGraphics().BeginScene();

		for(auto& action : m_actions)
		{
			action->render(context, interpolationTime);
		}

		context.m_services.getGraphics().EndScene();
	}
}
