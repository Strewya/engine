//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Games/Pong/InputHandler.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Engine/ServiceLocator.h"
#include "Games/Pong/Components.h"
//#include "Services/Input/Engine.h"
	/*** end headers ***/
	
namespace Pong
{
	void InputHandler::onUpdate(float dt)
	{
		//i should poll only a dt worth of input events here
		


	}

	bool InputHandler::validateEntity(Core::Entity& entity)
	{
		return entity.hasState(AvailableIntents::Type);
	}

	void InputHandler::init()
	{
	}
}