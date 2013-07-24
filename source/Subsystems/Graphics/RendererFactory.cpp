//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Subsystems/Graphics/RendererFactory.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Subsystems/Graphics/dll_header.h"
#include "Subsystems/Graphics/IRenderer.h"
#include "Win32/Window.h"
	/*** end headers ***/

namespace Graphics
{
	RendererFactory::RendererFactory(Win32::Window& window)
		: _hInst(window.getInstance()), _renderer(NULL), _dll(NULL), _hwnd(window.getWindowHandle())
	{}

	RendererFactory::~RendererFactory()
	{
		DestroyInterface();
		FreeLibrary(_dll);
	}

	IRenderer* RendererFactory::getInterface()
	{
		return _renderer;
	}

	void RendererFactory::InitInterface(const char* name)
	{
		if(strcmp(name, "dx") == 0)
		{
			_dll = LoadLibraryEx("DX_Renderer.dll", NULL, 0);
			if(!_dll)
			{
				MessageBox(_hwnd, "Error loading up DX_Renderer.dll", "Fatal Error", MB_OK | MB_ICONERROR);
				throw std::exception("Error loading up DX_Renderer.dll");
			}
		}
		else if(strcmp(name, "ogl") == 0)
		{
			_dll = LoadLibraryEx("OGL_Renderer.dll", NULL, 0);
			if(!_dll)
			{
				MessageBox(_hwnd, "Error loading up OGL_Renderer.dll", "Fatal Error", MB_OK | MB_ICONERROR);
				throw std::exception("Error loading up OGL_Renderer.dll");
			}
		}

		CREATE_RENDERER create = NULL;
		create = (CREATE_RENDERER)GetProcAddress(_dll, "createRendererInterface");
		if(create && !create(_hwnd, &_renderer))
		{
			FreeLibrary(_dll);
			MessageBox(_hwnd, "Error creating the rendering interface", "Fatal Error", MB_OK | MB_ICONERROR);
			throw std::exception("Error creating the rendering interface");
		}
	}

	void RendererFactory::DestroyInterface()
	{
		DESTROY_RENDERER destroy = NULL;

		destroy = (DESTROY_RENDERER)GetProcAddress(_dll, "destroyRendererInterface");
		if(!destroy(&_renderer))
		{
			FreeLibrary(_dll);
			MessageBox(_hwnd, "Error destroying the rendering interface", "Fatal Error", MB_OK | MB_ICONERROR);
			throw std::exception();
		}
		_renderer = NULL;
	}
}