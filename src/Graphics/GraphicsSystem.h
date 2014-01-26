#pragma once
/********************************************
*	class:	GraphicsSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <memory>
#include <string>
#include <vector>
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class Color;
	class Transform;
	class Vec2;
	class Window;

	class GraphicsSystem
	{
	public:
		bool init(Window& window);
		bool shutdown();

		void update();

		void begin();
		void present();

		bool initDevice();
		bool initSwapChain();
		bool initRenderTarget();
		bool initVertexShader(const std::string& shaderFile);
		bool initPixelShader(const std::string& shaderFile);
		bool initViewport();


		void setBackgroundColor(float red, float green, float blue);
		
		void drawLine(const Transform& transform, const Vec2* positions, uint32_t count, const Color& lineColor);
		void drawPolygon(const Transform& transform, const Vec2* positions, uint32_t count, const Color& fillColor);
		void drawQuad(const Transform& transform, const Vec2& halfSize, const Color& fillColor);



		

	private:
		template<typename T> static void ReleasePtr(T* ptr);
		template<typename T> static void SafeRelease(T** ptr);

		DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
		D3DXCOLOR m_backgroundColor;

		Window* m_window;
		IDXGIFactory* m_dxgiFactory;
		ID3D11Device* m_dev;
		ID3D11DeviceContext* m_devcon;
		IDXGISwapChain* m_swapchain;
		ID3D11RenderTargetView* m_renderTarget;
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_inputLayout;

		XMMATRIX m_camView;
		XMMATRIX m_camProjection;
		XMVECTOR m_camPosition;
		XMVECTOR m_camLookAt;
		XMVECTOR m_camUp;
		XMMATRIX m_world;
		
	};

	template<typename T> void GraphicsSystem::SafeRelease(T** ptr)
	{
		if(*ptr != nullptr)
		{
			ReleasePtr(*ptr);
			*ptr = nullptr;
		}
	}

	template<typename T> void GraphicsSystem::ReleasePtr(T* ptr)
	{
		if(ptr != nullptr)
		{
			ptr->Release();
		}
	}
}