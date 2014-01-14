//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/GraphicsSystem.h>
/******* C++ headers *******/
#include <iostream>
/******* extra headers *******/
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
	static void fillSwapChainDesc(DXGI_SWAP_CHAIN_DESC& scd, HWND hwnd, uint32_t width, uint32_t height);

	bool GraphicsSystem::init(Window& window)
	{
		std::cout << "GraphicsSystem init start" << std::endl;
		bool status = true;

		m_window = &window;

		fillSwapChainDesc(m_swapChainDesc, window.getWindowHandle(), window.getSizeX(), window.getSizeY());

		auto driverType = D3D_DRIVER_TYPE_HARDWARE;
		uint32_t flags = 0;

#ifdef _DEBUG
		flags = D3D11_CREATE_DEVICE_DEBUG;
#endif
		HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &m_swapChainDesc, &m_swapchain, &m_dev, nullptr, &m_devcon);
		if(SUCCEEDED(hr))
		{
			ID3D11Texture2D* bbTexture = nullptr;
			hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&bbTexture);
			if(SUCCEEDED(hr))
			{
				hr = m_dev->CreateRenderTargetView(bbTexture, nullptr, &m_renderTarget);
				if(SUCCEEDED(hr))
				{
					bbTexture->Release();

					D3D11_VIEWPORT vp;
					ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
					vp.TopLeftX = 0;
					vp.TopLeftY = 0;
					vp.Width = (float)window.getSizeX();
					vp.Height = (float)window.getSizeY();
					vp.MinDepth = 0;
					vp.MaxDepth = 1;

					m_devcon->RSSetViewports(1, &vp);
				}
				else
					status = false;
			}
			else
				status = false;
		}
		else
			status = false;

		m_backgroundColor.r = m_backgroundColor.g = m_backgroundColor.b = 0;

		std::cout << "GraphicsSystem init " << (status ? "OK" : "FAIL") << std::endl;
		return status;
	}

	bool GraphicsSystem::shutdown()
	{
		std::cout << "GraphicsSystem shutdown start" << std::endl;
		bool status = true;

		if(m_swapchain != nullptr)
		{
			m_swapchain->SetFullscreenState(false, nullptr);
		}
		SafeRelease(&m_renderTarget);
		SafeRelease(&m_swapchain);
		SafeRelease(&m_devcon);
		SafeRelease(&m_dev);

		std::cout << "GraphicsSystem shutdown " << (status ? "OK" : "FAIL") << std::endl;
		return status;
	}

	void GraphicsSystem::update()
	{
		m_devcon->ClearRenderTargetView(m_renderTarget, m_backgroundColor);

		//go through the command list and execute each one
		//each command is a special draw call for whatever needs to be drawn

		m_swapchain->Present(0, 0);
	}

	void GraphicsSystem::setBackgroundColor(float r, float g, float b)
	{
		m_backgroundColor.r = r;
		m_backgroundColor.g = g;
		m_backgroundColor.b = b;
	}
	



	void fillSwapChainDesc(DXGI_SWAP_CHAIN_DESC& scd, HWND hwnd, uint32_t width, uint32_t height)
	{
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
		scd.BufferCount = 1;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.OutputWindow = hwnd;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Windowed = true;

		DXGI_MODE_DESC& bd = scd.BufferDesc;
		bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bd.Width = width;
		bd.Height = height;
		bd.RefreshRate.Denominator = 1;
		bd.RefreshRate.Numerator = 60;
		bd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC& sd = scd.SampleDesc;
		sd.Count = 1;
		sd.Quality = 0;
	}
}