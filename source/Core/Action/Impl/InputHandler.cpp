//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Impl/InputHandler.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Engine/ServiceLocator.h"
#include "Engine/IntentSystem.h"
#include "Games/Pong/Components.h"
#include "Services/Input/Engine.h"
	/*** end headers ***/
	
namespace Core
{
	void InputHandler::frameUpdate(float dt)
	{
		//i should poll only a dt worth of input events here
		Input::Event e;
		while(m_context.services.getInput().nextEvent(e))
		{
			//pass the event to keybindings system
			Intent intent;
			if(m_context.keyBindings.findBinding(e, intent))
			{
				//if an intent is mapped
				if(intent.target != 0)
				{
					//the intent already has a target predetermined, so we do nothing
					m_context.intentSystem.generateIntent(intent);
				}
				else
				{
					//iterate over all entities registered, set them as target, and place intent into intent system
					for(auto eid : m_entities)
					{
						intent.target = eid;
						m_context.intentSystem.generateIntent(intent);
					}
				}
			}
		}
	}

	bool InputHandler::validateEntity(Core::Entity& entity) const
	{
		return true; //we accept any entity?
	}
}