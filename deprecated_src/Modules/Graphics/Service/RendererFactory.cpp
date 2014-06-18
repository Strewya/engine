//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Modules/Graphics/Service/RendererFactory.h>
	/*** C++ headers ***/
	/*** extra headers ***/
#include <Modules/Graphics/Service/dll_header.h>
#include <Modules/Graphics/Service/IRenderer.h>
#include <Win32/Window.h>
	/*** end headers ***/

namespace Graphics
{
	RendererFactory::RendererFactory(Win32::Window& window)
		: m_window(window), m_dll(nullptr), m_renderer(nullptr)
	{}

	RendererFactory::~RendererFactory()
	{
		destroyInterface();
	}

	IRenderer& RendererFactory::getInterface()
	{
		assert(m_renderer != nullptr);
		return *m_renderer;
	}

	bool RendererFactory::initInterface(const char* name)
	{
		return loadDll(name) && acquireInterface();
	}

	void RendererFactory::destroyInterface()
	{
		m_renderer = nullptr;
		FreeLibrary(m_dll);
	}

	bool RendererFactory::loadDll(const char* dllShortName)
	{
		std::string dllName;
		if(strcmp(dllShortName, "dx") == 0)
		{
			dllName.assign("DX_Renderer.dll");
		}/*
		else if(strcmp(dllShortName, "ogl") == 0)
		{
			dllName.assign("OGL_Renderer.dll");
		}*/


		bool loaded = false;
		if(!dllName.empty())
		{
			m_dll = LoadLibraryA(dllName.c_str());
			if(m_dll != nullptr)
			{
				loaded = true;
			}
		}

		if(!loaded)
		{
			std::string errMsg("Error loading DLL ");
			errMsg += dllName;
			MessageBox(m_window.getWindowHandle(), errMsg.c_str(), "Fatal Error", MB_OK | MB_ICONERROR);
			errMsg += ", errCode: ";
			errMsg += toString(GetLastError());
			Util::GetDefaultLogger() << errMsg << Util::Logger::endl;
		}
		return loaded;
	}

	bool RendererFactory::acquireInterface()
	{
		GET_RENDERER getFunction = (GET_RENDERER)GetProcAddress(m_dll, "getRendererInterface");
		
		bool rendererAcquired = false;
		if (getFunction != nullptr)
		{
			if (getFunction(&m_window, &m_renderer))
			{
				rendererAcquired = true;
			}
			else
			{
				std::string errMsg("Error creating IRenderer");
				MessageBox(m_window.getWindowHandle(), errMsg.c_str(), "Fatal Error", MB_OK | MB_ICONERROR);
				errMsg += ", errCode: ";
				errMsg += toString(GetLastError());
				Util::GetDefaultLogger() << errMsg << Util::Logger::endl;
			}
		}
		else
		{
			std::string errMsg("Error retrieving function addresses from DLL");
			MessageBox(m_window.getWindowHandle(), errMsg.c_str(), "Fatal Error", MB_OK | MB_ICONERROR);
			errMsg += ", errCode: ";
			errMsg += toString(GetLastError());
			errMsg += ", create = ";
			getFunction ? errMsg += "true" : errMsg += "false";
			Util::GetDefaultLogger() << errMsg << Util::Logger::endl;
		}

		return rendererAcquired;
	}
}
