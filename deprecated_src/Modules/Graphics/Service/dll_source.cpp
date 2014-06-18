	/*** precompiled header ***/
#include <stdafx.h>

#include <Modules/Graphics/Service/dll_header.h>
#include <Modules/Graphics/Service/IRenderer.h>
#include <Modules/Graphics/Service/D3D11/DXRenderer.h>
#include <Util/Logger.h>

EXPORT int getRendererInterface(Win32::Window* window, Graphics::IRenderer** renderer)
{
	if(renderer != nullptr)
	{
		try
		{
			static Graphics::DXRenderer dxRenderer(window);
			*renderer = &dxRenderer;
			return 1;
		}
		catch(std::exception& ex)
		{
			Util::GetDefaultLogger() << ex.what() << Util::Logger::endl;
		}
	}
	return 0;
}