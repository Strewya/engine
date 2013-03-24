#ifndef CORE_SERVICELOCATOR_H_
#define CORE_SERVICELOCATOR_H_
/********************************************
	class:	Services
	usage:	Wrapper class for all the core services a game
			should provide, such as input, graphics, sound
			persistence, communication, (scripting??) etc
********************************************/
	/*** common header ***/
#include "Defines.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Graphics { class IRenderer; }
namespace Script { class LuaEngine; }

namespace Core
{
	class Engine;

	class ServiceLocator
	{
	public:
		ServiceLocator();

		void Register(Script::LuaEngine* script);
		void Register(Graphics::IRenderer* graphics);
		void Register(Engine* engine);

		Script::LuaEngine& getLuaEngine() const;
		Graphics::IRenderer& getGraphics() const;
		Engine& getEngine() const;
	private:
		Engine* _engine;
		Graphics::IRenderer* _graphics;
		Script::LuaEngine* _script;
	};
}

#endif //CORE_SERVICELOCATOR_H_