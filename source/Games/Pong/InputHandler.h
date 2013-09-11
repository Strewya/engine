#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <functional>
#include <list>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/Action.h"
	/*** end header inclusion ***/

namespace Input { class Event; }

namespace Pong
{
	enum class Intent;

	SYSTEM(InputHandler)
	{
	public:
		SYSTEM_CTOR(InputHandler);

	protected:
		void onUpdate(float dt);
		void init();

	private:
		typedef std::function<Intent(Input::Event&)> IntentTranslator;

		std::list<IntentTranslator> m_intentTriggers;

		Intent attemptMapping(Input::Event& e);
	};
}