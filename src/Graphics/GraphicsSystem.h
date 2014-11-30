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
#include <Graphics/Vertex.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	enum TextJustification
	{
		TJ_Left,
		TJ_Center,
		TJ_Right
	};

	class Camera;
	class Circle;
	class Color;
	class FontCache;
	class Image;
	class Rect;
	class ResourceFile;
	class Texture;
	class TextureCache;
	class Transform;
	class Window;

	class GraphicsSystem
	{
	public:
		bool init(FontCache& fontCache, TextureCache& textureCache, Window& window);
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

		Vec2 getTextureDimensions(const Texture& texture) const;
		void setBackgroundColor(float red, float green, float blue);

		void setCulling(bool isEnabled);
		void setTransparencyMode(bool isEnabled);

		void setOrthographicProjection();
		void setPerspectiveProjection();

		void applyCamera(const Camera& camera);
		void clearCamera();

		Vec2 screenToWorld(const Vec2& screen, const Camera& camera) const;
		Vec2 textHalfSize(uint32_t fontID, const std::string& text) const;
		Transform justifyText(Transform textTf, float textSizeX, TextJustification justification);

		void drawLine(const Transform& transform, const Vec2& p1, const Vec2& p2, const Color& lineColor);
		void drawLine(const Transform& transform, const Vec2& p1, const Color& p1Color, const Vec2& p2, const Color& p2Color);
		void drawMultiline(const Transform& transform, const Vec2* positions, uint32_t count, const Color& lineColor);
		
		bool loadTexture(const ResourceFile& filename, Texture& outTexture);
		bool reloadTexture(const ResourceFile& filename, Texture& outTexture);
		void unloadTexture(Texture& texture);

		std::vector<Vertex> v3_makeCustomVertices(const Vec2* pos, uint32_t pts) const;
		std::vector<Vertex> v3_makeCircleVertices(const Vec2& pos, float radius, uint32_t pts) const;
		std::vector<Vertex> v3_makeQuadVertices(const Vec2& pos, const Vec2& halfSize) const;
		std::vector<Vertex> v3_makeTextVertices(uint32_t font, const std::string& text, bool italic);
		std::vector<uint32_t> v3_makeSolidPolygonIndices(uint32_t pts) const;
		std::vector<uint32_t> v3_makeHollowPolygonIndices(uint32_t pts) const;
		std::vector<uint32_t> v3_makeSolidCircleIndices(uint32_t pts) const;
		std::vector<uint32_t> v3_makeHollowCircleIndices(uint32_t pts) const;
		std::vector<uint32_t> v3_makeSolidQuadIndices() const;
		std::vector<uint32_t> v3_makeHollowQuadIndices() const;
		std::vector<uint32_t> v3_makeTextIndices(uint32_t letters) const;

		void v3_setVertices(const std::vector<Vertex>& vertices);
		void v3_setIndices(const std::vector<uint32_t>& indices);
		void v3_setTopology(D3D_PRIMITIVE_TOPOLOGY topology);
		void v3_setInstanceData(const std::vector<Transform>& tfs, const std::vector<Color>& fills, uint32_t startOffset, uint32_t count);
		void v3_setTexture(uint32_t textureId);
		void v3_setFontTexture(uint32_t fontId);
		void v3_draw(uint32_t indiceCount, uint32_t instanceCount);

	private:
		template<typename T> static void releasePtr(T* ptr);
		template<typename T> static void safeRelease(T*& ptr);
		template<typename T> void declare(T** ptr);
		XMMATRIX calculateCamView(const Camera& cam) const;
		

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
		ID3D11Buffer* m_constantBuffer;
		ID3D11Buffer* m_vertexBuffer;

		XMMATRIX m_camView;
		XMMATRIX m_camProjection;
		
		FontCache* m_fontCache;
		TextureCache* m_textureCache;

		//this is for automatic cleanup of all named DX objects
		std::vector<IUnknown**> m_dxInterfaces;

		typedef std::unique_ptr<ID3D11ShaderResourceView, void(*)(ID3D11ShaderResourceView*)> DxTexturePtr;
		std::vector<DxTexturePtr> m_textures;
		std::vector<Vec2> m_circleData;
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

	void drawHollowPolygon(GraphicsSystem& gfx, Transform transform, const Vec2* positions, uint32_t count, Color fillColor);
	void drawSolidPolygon(GraphicsSystem& gfx, Transform transform, const Vec2* positions, uint32_t count, Color fillColor);
	void drawHollowQuad(GraphicsSystem& gfx, Transform transform, Vec2 halfSize, Color color);
	void drawSolidQuad(GraphicsSystem& gfx, Transform transform, Vec2 halfSize, Color fillColor);
	void drawHollowCircle(GraphicsSystem& gfx, Transform transform, float radius, uint32_t points, Color color);
	void drawSolidCircle(GraphicsSystem& gfx, Transform transform, float radius, uint32_t points, Color color);
	void drawText(GraphicsSystem& gfx, uint32_t fontID, const std::string& text, Transform transform, Color tint, TextJustification justification, bool isItalic);
}