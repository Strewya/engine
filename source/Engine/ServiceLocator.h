#pragma once
/********************************************
	class:	Services
	usage:	Wrapper class for all the core services a game
			should provide, such as input, graphics, sound
			persistence, communication, (scripting??) etc
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Graphics { class IRenderer; }
namespace Input { class Engine; }
namespace Script { class Engine; }

namespace Core
{
	class Engine;

	class ServiceLocator
	{
	public:
		ServiceLocator();

		void Register(Input::Engine* input);
		void Register(Script::Engine* script);
		void Register(Graphics::IRenderer* graphics);
		void Register(Engine* engine);

		Input::Engine& getInput() const;
		Script::Engine& getScript() const;
		Graphics::IRenderer& getGraphics() const;
		Engine& getEngine() const;
	private:
		Engine* _engine;
		Graphics::IRenderer* _graphics;
		Script::Engine* _script;
		Input::Engine* _input;
	};
}