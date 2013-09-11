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
		: m_window(window), m_dll(nullptr), m_renderer(nullptr)
	{}

	RendererFactory::~RendererFactory()
	{
		destroyInterface();
		FreeLibrary(m_dll);
	}

	IRenderer& RendererFactory::getInterface()
	{
		assert(m_renderer != nullptr);
		return *m_renderer;
	}

	bool RendererFactory::initInterface(const char* name)
	{
		return loadDll(name) && createInterface();
	}

	void RendererFactory::destroyInterface()
	{
		m_renderer.reset(nullptr);
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

	bool RendererFactory::createInterface()
	{
		CREATE_RENDERER createFunction = (CREATE_RENDERER)GetProcAddress(m_dll, "createRendererInterface");
		DESTROY_RENDERER destroyFunction = (DESTROY_RENDERER)GetProcAddress(m_dll, "destroyRendererInterface");

		bool success = false;
		if(createFunction != nullptr && destroyFunction != nullptr)
		{
			IRenderer* render = nullptr;
			if(createFunction(m_window.getWindowHandle(), m_window.getSizeX(), m_window.getSizeY(), &render))
			{
				m_renderer.reset(render);
				m_renderer.get_deleter() = [=] (IRenderer* r) { destroyFunction(r); };
				success = true;
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
			errMsg += ", create|destroy = ";
			createFunction ? errMsg += "true" : errMsg += "false";
			errMsg += "|";
			destroyFunction ? errMsg += "true" : errMsg += "false";
			Util::GetDefaultLogger() << errMsg << Util::Logger::endl;
		}

		return success;
	}
}
