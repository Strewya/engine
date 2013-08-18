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
	protected:
		HWND _hwnd;
		HINSTANCE _hInst;
		HMODULE _dll;
		IRenderer* _renderer;


	public:

		RendererFactory(Win32::Window& window);
		~RendererFactory();

		bool InitInterface(const char* name);
		bool InitInterface(const String& name) { return InitInterface(name.c_str()); };
		IRenderer* getInterface();
		void DestroyInterface();
	};
}
