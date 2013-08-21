	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Games/GameInit.h"
	/*** extra headers ***/
#include "Engine/Engine.h"
#include "Games/Pong/EntityConstructors.h"
#include "Games/Pong/Gameplay.h"
	/*** end headers ***/

namespace Game
{
	using namespace Pong;
	void init(Core::Engine& engine)
	{
		/*
			This function should only do things that should be done once per execution of the program,
			such as registering objects, callbacks, functions etc.
		*/

		//get the context
		Core::GameContext& context = engine.registerContext(new Gameplay(engine.getServices(), engine.getResources()));
		
		//initialize basic context data
		context.registerActions();
		context.setupActionQueue();
		
		context.entityFactory.registerConstructor("paddle", createPaddle);
		context.entityFactory.registerConstructor("ball", createBall);
		context.entityFactory.registerConstructor("wall", createWall);
		context.entityFactory.registerConstructor("goal", createGoal);

		//set the context as active
		engine.setActiveContext(context);
	}
}