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
		HINSTANCE _hInst;
		HMODULE _dll;
		IRenderer* _renderer;
		HWND _hwnd;

	public:

		RendererFactory(Win32::Window& window);
		~RendererFactory();

		void InitInterface(const char* name);
		void InitInterface(const String& name) { InitInterface(name.c_str()); };
		IRenderer* getInterface();
		void DestroyInterface();
	};
}