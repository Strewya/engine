//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Action.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Engine/ServiceLocator.h"
#include "Services/Graphics/IRenderer.h"
	/*** end headers ***/

namespace Core
{
	void Multiaction::update(GameContext& context)
	{
		for(auto& action : m_actions)
		{
			action->update(context);
		}
	}

	void Multiaction::render(GameContext& context, uint64_t interpolationTime)
	{
		for(auto& action : m_actions)
		{
			action->render(context, interpolationTime);
		}
	}
}