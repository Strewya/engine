#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <windows.h>
	/*** extra headers if needed (alphabetically ordered) ***/

	/*** end header inclusion ***/

namespace Win32 { class Window; }

namespace Graphics
{
	class IRenderer;

	class RendererFactory
	{
	public:
		RendererFactory(Win32::Window& window);
		~RendererFactory();

		bool initInterface(const char* name);
		bool initInterface(const std::string& name) { return initInterface(name.c_str()); };
		IRenderer& getInterface();
		void destroyInterface();

	protected:
		Win32::Window& m_window;
		HMODULE m_dll;
		std::unique_ptr<IRenderer, std::function<void(IRenderer*)>> m_renderer;
		
		bool loadDll(const char* name);
		bool createInterface();
	};
}
