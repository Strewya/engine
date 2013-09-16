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
		//i should poll only a dt worth of input events here
		m_context.services.getInput().update();


	}

	bool InputHandler::validateEntity(Core::Entity& entity)
	{
		return entity.hasState(AvailableIntents::Type);
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