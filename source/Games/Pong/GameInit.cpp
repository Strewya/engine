	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Games/GameInit.h>
	/*** extra headers ***/
#include <Engine/Engine.h>
#include <Games/Pong/Gameplay.h>
	/*** end headers ***/

namespace Game
{
	using namespace Pong;
	void init(Core::Engine& engine)
	{
		/*
			This function should only do things that should be done once per execution of the program,
			and it should only create the necessary contexts, and have them do the rest of the initialization.
		*/

		//get the context
		Core::GameContext& gameplay = engine.registerContext<Gameplay>();
		
		//set the context as active
		engine.setActiveContext(gameplay);
	}
}