//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Impl/Input.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Services/Input/InputEngine.h"
	/*** end headers ***/
	
namespace Core
{
	void AInput::onUpdate(float dt)
	{
		Input::Event event;
		while(m_context.services.getInput().EatEvent(event))
		{
			////////// PONG //////////
			if(event.type == Input::EventType::KeyPressed)
			{
				if(event.key.code == Input::Keyboard::_ArrowUp)
				{

				}
			}

		}
	}
}