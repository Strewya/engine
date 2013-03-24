#ifndef SUBSYSTEMS_GRAPHICS_RENDERERFACTORY_H_
#define SUBSYSTEMS_GRAPHICS_RENDERERFACTORY_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <windows.h>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

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

		RendererFactory(HINSTANCE hInst, HWND hwnd);
		~RendererFactory();

		void InitInterface(const char* name);
		void InitInterface(const String& name) { InitInterface(name.c_str()); };
		IRenderer* getInterface();
		void DestroyInterface();
	};
}

#endif //SUBSYSTEMS_GRAPHICS_RENDERERFACTORY_H_