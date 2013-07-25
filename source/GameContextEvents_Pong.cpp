//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "GameContextEvents_Pong.h"
	/*** C++ headers ***/
#include <map>
#include <memory>
#include <vector>
	/*** extra headers ***/
#include "Engine/Engine.h"
#include "Engine/GameContext.h"
#include "Core/Action/Action.h"
#include "Core/Action/Impl/Render.h"
#include "Core/Entity/Entity.h"
#include "Core/State/State.h"
#include "Subsystems/Graphics/IRenderer.h"
#include "Subsystems/Graphics/ITextureCache.h"
#include "Subsystems/Graphics/Spritesheet.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Input/InputEngine.h"
#include "Subsystems/Script/LuaEngine.h"
#include "Util/Color.h"

//temporary
#include "Subsystems/Graphics/Vertex.h"
	/*** end headers ***/

namespace Pong
{
	bool createPaddle(Core::GameContext& context, Core::Entity& paddle)
	{
		
		return false;
	}

	bool createBall(Core::GameContext& context, Core::Entity& ball)
	{
		
		return false;
	}

	void registerActionsToIndex(Core::GameContext& context)
	{
		context.actionIndex.addActionToIndex(Core::ARender::create());
	}

	void setupActionQueue(Core::GameContext& context)
	{
		context.actionQueue.addAction(context.actionIndex.getActionFromIndex(Core::ARender::Type));
	}

	void initPong(Core::Engine& engine)
	{
		Core::GameContext& context = engine.getContext("main");
		engine.pushContext(context);

		registerActionsToIndex(context);
		setupActionQueue(context);
		
		context.services.getGraphics().setBackgroundFillColor(127,127,127);

		context.entityFactory.registerConstructor("paddle", createPaddle);
		context.entityFactory.registerConstructor("ball", createBall);

	}
}