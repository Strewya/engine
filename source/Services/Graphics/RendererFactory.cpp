//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Graphics/RendererFactory.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Services/Graphics/dll_header.h"
#include "Services/Graphics/IRenderer.h"
#include "Win32/Window.h"
	/*** end headers ***/

namespace Graphics
{
	RendererFactory::RendererFactory(Win32::Window& window)
		: _window(window), _dll(nullptr), _renderer(nullptr)
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

	bool RendererFactory::InitInterface(const char* name)
	{
		if(strcmp(name, "dx") == 0)
		{
			_dll = LoadLibraryA("DX_Renderer.dll");
			if(!_dll)
			{
				Util::GetDefaultLogger() << "Error loading DLL, errCode: " << GetLastError() << Util::Logger::endl;
				MessageBox(_window.getWindowHandle(), "Error loading up DX_Renderer.dll", "Fatal Error", MB_OK | MB_ICONERROR);
				return false;
			}
		}
		else if(strcmp(name, "ogl") == 0)
		{
			_dll = LoadLibraryA("OGL_Renderer.dll");
			if(!_dll)
			{
				Util::GetDefaultLogger() << "Error loading DLL, errCode: " << GetLastError() << Util::Logger::endl;
				MessageBox(_window.getWindowHandle(), "Error loading up OGL_Renderer.dll", "Fatal Error", MB_OK | MB_ICONERROR);
				return false;
			}
		}
		else
		{
			return false;
		}

		CREATE_RENDERER create = nullptr;
		create = (CREATE_RENDERER)GetProcAddress(_dll, "createRendererInterface");
		if(!create || !create(_window.getWindowHandle(), _window.getSizeX(), _window.getSizeY(), &_renderer))
		{
			Util::GetDefaultLogger() << "Error with creating IRenderer, errCode: " << GetLastError() << Util::Logger::endl;
			FreeLibrary(_dll);
			MessageBox(_window.getWindowHandle(), "Error creating the rendering interface", "Fatal Error", MB_OK | MB_ICONERROR);
			return false;
		}
		return true;
	}

	void RendererFactory::DestroyInterface()
	{
		DESTROY_RENDERER destroy = nullptr;

		destroy = (DESTROY_RENDERER)GetProcAddress(_dll, "destroyRendererInterface");
		if(!destroy(_renderer))
		{
			FreeLibrary(_dll);
			MessageBox(_window.getWindowHandle(), "Error destroying the rendering interface", "Fatal Error", MB_OK | MB_ICONERROR);
		}
		_renderer = nullptr;
	}
}
