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
#include "Services/Input/InputEngine.h"
	/*** end headers ***/
	
namespace Pong
{
	void InputHandler::onUpdate(float dt)
	{
		m_context.services.getInput().update();

		//i should poll only a dt worth of input events here
		Input::Event event;
		while(m_context.services.getInput().eatEvent(event))
		{
			Intent i = attemptMapping(event);
			if(i >= Intent::INTENT_COUNT)
			{
				continue;
			}

			for(auto eid : m_entities)
			{
				if(m_context.entityPool.isAlive(eid) == true)
				{
					auto& entity = m_context.entityPool.getInstanceRef(eid);
					if(entity.hasState(AvailableIntents::Type) == true)
					{
						auto* availInt = entity.getState<AvailableIntents>();
						uint32_t actionId = availInt->value[(uint32_t)i];
						if(actionId > 0)
						{
							m_context.actionIndex.getActionFromIndex(actionId).registerEntity(eid);
						}
					}
				}
			}
		}
	}

	void InputHandler::init()
	{
		// create mappings for pong
		m_intentTriggers.push_back( [](Input::Event& e) -> Intent
		{
			if(e.type == Input::EventType::KeyPressed &&
				e.key.code == Input::Keyboard::_W)
			{
				return Intent::LeftMoveUp;
			}
			return Intent::NotMapped;
		});
		m_intentTriggers.push_back( [](Input::Event& e) -> Intent
		{
			if(e.type == Input::EventType::KeyPressed &&
				e.key.code == Input::Keyboard::_S)
			{
				return Intent::LeftMoveDown;
			}
			return Intent::NotMapped;
		});

		m_intentTriggers.push_back( [](Input::Event& e) -> Intent
		{
			if(e.type == Input::EventType::KeyPressed &&
				e.key.code == Input::Keyboard::_ArrowUp)
			{
				return Intent::RightMoveUp;
			}
			return Intent::NotMapped;
		});
		m_intentTriggers.push_back( [](Input::Event& e) -> Intent
		{
			if(e.type == Input::EventType::KeyPressed &&
				e.key.code == Input::Keyboard::_ArrowDown)
			{
				return Intent::RightMoveDown;
			}
			return Intent::NotMapped;
		});

		m_intentTriggers.push_back( [](Input::Event& e) -> Intent
		{
			if(e.type == Input::EventType::KeyPressed &&
				e.key.code == Input::Keyboard::_Enter)
			{
				return Intent::ResetBoard;
			}
			return Intent::NotMapped;
		});
	}

	Intent InputHandler::attemptMapping(Input::Event& e)
	{
		for(auto& trigger : m_intentTriggers)
		{
			Intent i = trigger(e);
			if(i < Intent::INTENT_COUNT)
			{
				return i;
			}
		}
		return Intent::NotMapped;
	}
}