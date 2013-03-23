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
	class Interface;

	class RendererFactory
	{
	protected:
		HINSTANCE _hInst;
		HMODULE _dll;
		Interface* _renderer;
		HWND _hwnd;

	public:

		RendererFactory(HINSTANCE hInst, HWND hwnd);
		~RendererFactory();

		void InitInterface(const char* name);
		void InitInterface(const String& name) { InitInterface(name.c_str()); };
		Interface* getInterface();
		void DestroyInterface();
	};
}

#endif //SUBSYSTEMS_GRAPHICS_RENDERERFACTORY_H_