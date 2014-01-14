#pragma once
/********************************************
*	class:	GraphicsSystem
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/DXInclude.h>
/******* end header inclusion *******/

namespace Core
{
	class Time;
	class Window;

	class GraphicsSystem
	{
	public:
		bool init(Window& window);
		bool shutdown();

		void update();

		void setBackgroundColor(float red, float green, float blue);

	private:
		DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
		D3DXCOLOR m_backgroundColor;

		Window* m_window;
		ID3D11Device* m_dev;
		ID3D11DeviceContext* m_devcon;
		IDXGISwapChain* m_swapchain;
		ID3D11RenderTargetView* m_renderTarget;

		template<typename T> void SafeRelease(T** ptr);
	};


	template<typename T> void GraphicsSystem::SafeRelease(T** ptr)
	{
		if(*ptr != nullptr)
		{
			(*ptr)->Release();
			*ptr = nullptr;
		}
	}
}