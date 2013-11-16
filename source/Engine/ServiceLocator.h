#pragma once
/********************************************
	class:	Services
	usage:	Wrapper class for all the core services a game
			should provide, such as input, graphics, sound
			persistence, communication, (scripting??) etc
********************************************/
	/*** common header ***/
#include <Engine/Defines.h>
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

		void bind(Core::Engine& engine);
		void bind(Graphics::IRenderer& graphics);
		void bind(Input::Engine& input);
		void bind(Script::Engine& script);
		
		Input::Engine& getInput() const;
		Script::Engine& getScript() const;
		Graphics::IRenderer& getGraphics() const;
		Core::Engine& getEngine() const;
		
	private:
		Core::Engine* m_engine;
		Graphics::IRenderer* m_graphics;
		Input::Engine* m_input;
		Script::Engine* m_script;
	};
}
