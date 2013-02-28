
#include "dll_header.h"
#include "Subsystems/Graphics/Interface.h"
#include "Subsystems/Graphics/DirectX/DXRenderer.h"

int createRendererInterface(HWND hwnd, Graphics::Interface** renderer)
{
	if(!*renderer)
	{
		try
		{
			*renderer = new Graphics::DXRenderer(hwnd);
			return 1;
		}
		catch(std::exception& ex)
		{
			ex;
		}
	}
	return 0;
}

int destroyRendererInterface(Graphics::Interface** renderer)
{
	if(!*renderer)
	{
		return 0;
	}
	delete *renderer;
	*renderer = NULL;
	return 1;
}