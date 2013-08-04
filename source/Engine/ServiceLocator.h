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
namespace Core { class Engine; }
namespace Graphics { class IRenderer; }
namespace Input { class Engine; }
namespace Script { class Engine; }

namespace Core
{
	class ServiceLocator
	{
	public:
		ServiceLocator();

		void Register(Core::Engine* engine);
		void Register(Graphics::IRenderer* graphics);
		void Register(Input::Engine* input);
		void Register(Script::Engine* script);



		Input::Engine& getInput() const;
		Script::Engine& getScript() const;
		Graphics::IRenderer& getGraphics() const;
		Core::Engine& getEngine() const;
	private:
		Engine* _engine;
		Graphics::IRenderer* _graphics;
		Input::Engine* _input;
		Script::Engine* _script;

	};
}
