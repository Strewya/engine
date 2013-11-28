#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <functional>
#include <list>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Core/Action/Action.h>
	/*** end header inclusion ***/

namespace Input { class Event; }

namespace Core
{
	struct Intent;

	class InputController : public AtomicAction<InputController>
	{
	public:
		typedef std::function<void(GameContext&,const Intent&)> IntentHandler;

		void processMessage(GameContext& context, uint32_t msg, InstanceID entity);
		void update(GameContext& context);
		void render(GameContext& context, uint64_t interpolationTime);

		void getInputEvents(GameContext& context, uint64_t maxTimestamp, std::vector<Input::Event>& outEvents) const;
		void mapInputToIntents(GameContext& context, const std::vector<Input::Event>& events, std::vector<Intent>& outIntents) const;
		void processIntents(GameContext& context, const std::vector<Intent>& outIntents) const;
		void postMessage(GameContext& context, const Intent& i) const;

		void setIntentHandler(const IntentHandler& handler);

	protected:
		IntentHandler m_intentHandler;
	};
}