	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Engine/GameInit.h>
	/*** extra headers ***/
#include <Engine/Engine.h>
#include <Games/Pong/Gameplay.h>
	/*** end headers ***/

namespace Core
{
	void init(Core::Engine& engine)
	{
		/*
			This function should only do things that should be done once per execution of the program,
			and it should only create the necessary contexts, and have them do the rest of the initialization.
		*/

		//get the context
		GameContext& gameplay = engine.registerContext<Pong::Gameplay>();
		
		//set the context as active
		engine.setActiveContext(gameplay);
	}
}