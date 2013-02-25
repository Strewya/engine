#ifndef CORE_SERVICE_LOCATOR_H_
#define CORE_SERVICE_LOCATOR_H_
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

namespace Graphics { class Interface; }

namespace Core
{
	class Engine;

	class ServiceLocator
	{
	public:
		ServiceLocator();

		void Register(Graphics::Interface* graphics);
		void Register(Engine* engine);

		Graphics::Interface& getGraphics() const;
		Engine& getEngine() const;
	private:
		Engine* _engine;
		Graphics::Interface* _graphics;

	};
}

#endif //CORE_SERVICE_LOCATOR_H_