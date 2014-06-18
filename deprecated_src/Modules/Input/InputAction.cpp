//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Modules/Input/InputAction.h>
	/*** C++ headers ***/
	/*** extra headers ***/
#include <Engine/GameContext.h>
#include <Engine/ServiceLocator.h>
#include <Engine/Intent.h>
#include <Modules/Input/Service/InputEngine.h>
	/*** end headers ***/
	
namespace Core
{
	void InputController::processMessage(GameContext& context, uint32_t msg, InstanceID entity)
	{
	}

	void InputController::update(GameContext& context)
	{
		std::vector<Input::Event> inputEvents;
		getInputEvents(context, context.m_timer.getCurMicros(), inputEvents);
		
		std::vector<Intent> intents;
		mapInputToIntents(context, inputEvents, intents);
		
		processIntents(context, intents);
	}

	void InputController::render(GameContext& context, uint64_t interpolationTime)
	{}

	void InputController::getInputEvents(GameContext& context, uint64_t maxTimestamp, std::vector<Input::Event>& outEvents) const
	{
		//poll only input events up until the current point in time
		context.m_services.getInput().getEvents(maxTimestamp, outEvents);
	}

	void InputController::mapInputToIntents(GameContext& context, const std::vector<Input::Event>& events, std::vector<Intent>& outIntents) const
	{
		auto doMapping = [&](const Input::Event& e)
		{
			Intent i;
			if(context.m_keyBindings.findBinding(e, i))
			{
				outIntents.emplace_back(i);
			}
		};
		std::for_each(events.begin(), events.end(), doMapping);
	}

	void InputController::processIntents(GameContext& context, const std::vector<Intent>& intents) const
	{
		for(auto intent : intents)
		{
			//the intent already has a target predetermined, so we do nothing
			if(intent.target != 0)
			{
				postMessage(context, intent);
			}
			//iterate over all entities registered, set them as target, and place intent into intent system
			else
			{	
				for(auto eid : m_entities)
				{
					intent.target = eid;
					postMessage(context, intent);
				}
			}
		}
	}

	void InputController::postMessage(GameContext& context, const Intent& i) const
	{
		if (m_intentHandler == nullptr)
		{
			context.m_messenger.sendMessage(UID, context.m_messenger.BROADCAST, i.msgId, i.target);
		}
		else
		{
			m_intentHandler(context, i);
		}
	}

	void InputController::setIntentHandler(const IntentHandler& handler)
	{
		m_intentHandler = handler;
	}
}