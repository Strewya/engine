#pragma once
/********************************************
*	class:	GraphicsSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <cassert>
#include <memory>
#include <string>
#include <vector>
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
#include <DataStructs/Font.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class Camera;
	class Color;
	class DataFile;
	class Image;
	class Rect;
	class ResourceFile;
	class TextureCache;
	class Transform;
	class Window;

	class GraphicsSystem
	{
	public:
		bool init(TextureCache& texCache, Window& window);
		bool shutdown();

		void update();

		void begin();
		void present();

		bool initDevice();
		bool initSwapChain();
		bool initRenderTarget();
		bool initVertexShader(const ResourceFile& shaderFile);
		bool initPixelShader(const ResourceFile& shaderFile);
		bool initViewport();
		bool initSamplerState();
		bool initDepthBuffer();
		bool initFont(DataFile& file);

		Vec2 getTextureDimensions(uint32_t texID) const;
		void setBackgroundColor(float red, float green, float blue);

		void setCulling(bool isEnabled);
		void setTransparencyMode(bool isEnabled);

		void setOrthographicProjection();
		void setPerspectiveProjection();

		void applyCamera(const Camera& camera);
		void clearCamera();

		void drawLine(const Transform& transform, const Vec2* positions, uint32_t count, const Color& lineColor);
		void drawPolygon(const Transform& transform, const Vec2* positions, uint32_t count, const Color& fillColor);
		void drawPolygon(const Transform& transform, const Rect& rect, const Color& color);
		void drawQuad(const Transform& transform, const Vec2& halfSize, const Color& fillColor);
		void drawTexturedQuad(const Transform& transform, const Color& fillColor, const Image& image);

		//justification is 0 for left, 1 for center, 2 for right, all other values are treated as 0
		void drawText(const std::string& text, const Transform& transform, const Color& tint, uint32_t justification, bool isItalic);

		uint32_t loadTextureFromFile(const ResourceFile& filename);
		void releaseTexture(uint32_t id);

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
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11BlendState* m_transparency;
		
		XMMATRIX m_camView;
		XMMATRIX m_camProjection;
		XMMATRIX m_world;
		
		//this shouldn't be explicit like this, refactor later
		Font m_font;
		TextureCache* m_textureCache;

		//this is for automatic cleanup of all named DX objects
		std::vector<IUnknown**> m_dxInterfaces;

		typedef std::unique_ptr<ID3D11ShaderResourceView, void(*)(ID3D11ShaderResourceView*)> DxTexturePtr;
		std::vector<DxTexturePtr> m_textures;
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
		m_dxInterfaces.emplace_back((IUnknown**)ptr);
	}
}