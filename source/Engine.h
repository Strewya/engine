#ifndef CORE_ENGINE_H_
#define CORE_ENGINE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/RendererFactory.h"
	/*** end header inclusion ***/

namespace Win32
{
	class AbstractWindow;
}

namespace Core
{
	class Engine
	{
	public:
		Engine(const Win32::AbstractWindow& window);
		void Loop();
		void Shutdown();

		const Win32::AbstractWindow& getWindow() const;

	private:
		const Win32::AbstractWindow& _window;
		Graphics::RendererFactory _rendererFactory;
	};
}

#endif //CORE_ENGINE_H_