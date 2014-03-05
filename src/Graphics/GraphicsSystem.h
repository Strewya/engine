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
#include <Util/Dimensional.h>
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

	class Image
	{
	public:
		Vec2 m_texCoords[4];
		float m_ratio;
		std::string m_name;
	};

	class Animation
	{
	public:
		std::string m_name;
		std::vector<uint32_t> m_images;
		uint64_t m_duration;
		bool m_isLooped;
	};

	class Sheet
	{
	public:
		std::string m_name;
		std::string m_textureName;
		std::vector<Image> m_images;
		std::vector<Animation> m_animations;
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
		bool initSpritesheet(DataFile& file);

		
		void setBackgroundColor(float red, float green, float blue);

		uint32_t getAnimationIndex(const char* name) const;
		const Animation& getAnimation(uint32_t index) const;
		
		void drawLine(const Transform& transform, const Vec2* positions, uint32_t count, const Color& lineColor);
		void drawPolygon(const Transform& transform, const Vec2* positions, uint32_t count, const Color& fillColor);
		void drawQuad(const Transform& transform, const Vec2& halfSize, const Color& fillColor);
		void drawTexturedQuad(const Transform& transform, const Color& fillColor, uint32_t imageIndex);

		//justification is 0 for left, 1 for center, 2 for right, all other values are treated as 0
		void drawText(const std::string& text, const Transform& transform, const Color& tint, uint32_t justification);

	private:
		template<typename T> static void releasePtr(T* ptr);
		template<typename T> static void safeRelease(T*& ptr);
		template<typename T> void declare(T** ptr);

		uint32_t loadTextureFromFile(const char* filename);

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
		uint32_t m_fontTextureID;
		Font m_font;

		uint32_t m_sheetTextureID;
		Sheet m_sheet;

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