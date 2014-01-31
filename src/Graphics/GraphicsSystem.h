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
	class DataFile;
	class Transform;
	class Vec2;
	class Window;

	class Glyph
	{
	public:
		uint32_t m_ascii;
		uint32_t m_left;
		uint32_t m_right;
		uint32_t m_top;
		char m_character;
	};

	class Font
	{
	public:
		uint32_t m_size;
		std::string m_name;
		std::string m_texture;
		std::vector<Glyph> m_glyphs;
	};

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
		bool initVertexShader(const char* shaderFile);
		bool initPixelShader(const char* shaderFile);
		bool initViewport();
		bool initSamplerState();
		bool initFont(DataFile& file);


		void setBackgroundColor(float red, float green, float blue);
		
		void drawLine(const Transform& transform, const Vec2* positions, uint32_t count, const Color& lineColor);
		void drawPolygon(const Transform& transform, const Vec2* positions, uint32_t count, const Color& fillColor);
		void drawQuad(const Transform& transform, const Vec2& halfSize, const Color& fillColor);

		//justification is 0 for left, 1 for center, 2 for right, all other values are treated as 0
		void drawText(const std::string& text, const Transform& transform, const Color& tint, uint32_t justification);

	private:
		template<typename T> static void releasePtr(T* ptr);
		template<typename T> static void safeRelease(T*& ptr);
		template<typename T> void declare(T** ptr);


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
		ID3D11SamplerState* m_samplerState;
		
		XMMATRIX m_camView;
		XMMATRIX m_camProjection;
		XMVECTOR m_camPosition;
		XMVECTOR m_camLookAt;
		XMVECTOR m_camUp;
		XMMATRIX m_world;
		
		//this shouldn't be explicit like this, refactor later
		ID3D11ShaderResourceView* m_fontTexture;
		Font m_font;

		std::vector<IUnknown*> m_dxResources;
	};

	template<typename T> void GraphicsSystem::safeRelease(T*& ptr)
	{
		if(ptr != nullptr)
		{
			releasePtr(ptr);
			ptr = nullptr;
		}
	}

	template<typename T> void GraphicsSystem::releasePtr(T* ptr)
	{
		if(ptr != nullptr)
		{
			ptr->Release();
		}
	}

	template<typename T> void GraphicsSystem::declare(T** ptr)
	{
		assert(ptr != nullptr);
		*ptr = nullptr;
		m_dxResources.emplace_back(*ptr);
	}
}