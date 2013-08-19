#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <deque>
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Services/Graphics/IRenderer.h"
#include "Services/Graphics/D3D11/DXInclude.h"
#include "Services/Graphics/D3D11/IndexBuffer.h"
#include "Services/Graphics/D3D11/VertexBuffer.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class DXRenderer : public IRenderer
	{
	public: //list all public methods which need to be exposed through the interface
		DXRenderer(HWND hwnd, uint32_t screenW, uint32_t screenH);
		~DXRenderer();

		bool BeginScene();
		void EndScene();

		void testDraw();

		Util::Vec2 getScreenSize() const;
		void setScreenSize(const Util::Vec2& size);
		void setScreenSize(uint32_t width, uint32_t height);

		Util::Color getBackgroundColor() const;
		void setBackgroundColor(const Util::Color& color);
		void setBackgroundColor(float red, float green, float blue);
		
		bool getFullscreenState() const;
		void setFullscreenState(bool state);

		uint32_t createVertexBuffer(uint32_t size);
		IVertexBuffer& getVertexBuffer(uint32_t id);
		void destroyVertexBuffer(uint32_t id);

		uint32_t createIndexBuffer(uint32_t size);
		IIndexBuffer& getIndexBuffer(uint32_t id);
		void destroyIndexBuffer(uint32_t id);

		void setIdentity();
		void setTranslation(const Util::Vec3& translation);
		void setScaling(const Util::Vec3& scale);
		void setRotation(const Util::Vec3& rotation);
		void applyTransform();
		

	protected: //list all methods that need not be exposed outward, NOTE this is just for code readability purposes
		bool init();
		void fillSwapChainDesc(DXGI_SWAP_CHAIN_DESC& scd);
		bool initPipeline();
		bool reset();
		void close();

		ID3D11Buffer* _makeVertexBuffer(uint32_t size);
		ID3D11Buffer* _makeIndexBuffer(uint32_t size);

	private: //all private members that must not be accessible from the outside
		//configuration
		uint32_t _screenW;
		uint32_t _screenH;
		DXGI_SWAP_CHAIN_DESC _swapChainDesc;

		//members
		HWND _hwnd;
		ID3D11Device* _dev;
		ID3D11DeviceContext* _devcon;
		IDXGISwapChain* _swapchain;
		ID3D11RenderTargetView* _renderTarget;
		ID3D10Blob* _vertexShaderBlob;
		ID3D11VertexShader* _vertexShader;
		ID3D10Blob* _pixelShaderBlob;
		ID3D11PixelShader* _pixelShader;
		ID3D11RasterizerState* _cwCulling;
		ID3D11RasterizerState* _ccwCulling;
		ID3D11RasterizerState* _noCulling;
		
		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;
		ID3D11DepthStencilView* _depthStencilView;
		ID3D11Texture2D* _depthStencilBuffer;
		ID3D11Buffer* _cbPerObjectBuffer;
		ID3D11ShaderResourceView* _shaderResourceView;
		ID3D11SamplerState* _samplerState;
		ID3D11BlendState* _blendState;

		
		D3DXCOLOR _backgroundColor;
		
		XMMATRIX _camView;
		XMMATRIX _camProjection;
		XMVECTOR _camPosition;
		XMVECTOR _camLookAt;
		XMVECTOR _camUp;
		XMMATRIX _world;


		std::deque<std::unique_ptr<VertexBuffer>> _vertexBuffers;
		std::deque<std::unique_ptr<IndexBuffer>> _indexBuffers;
		ID3D11InputLayout* _inputLayout;


		/*
	private:
		DXRenderer(DXRenderer& dxg);
		DXRenderer& operator=(DXRenderer& dxg);

		
		
		HWND _hwnd;
		LPDIRECT3D9 _d3d;
		LPDIRECT3DDEVICE9 _d3ddev;
		LPDIRECT3DSURFACE9 _backbuffer;
		LPD3DXSPRITE _spriteHandler;
		LPD3DXLINE _line;
		
		D3DPRESENT_PARAMETERS _d3dpp;
		
		DXTextureCache _textures;
		LPDIRECT3DTEXTURE9 _defaultTexture;
		std::set<uint32_t> _freeTextureSlots;
		DXFontCache _fonts;

		D3DXMATRIX _transformMatrix;
		DWORD _fontStyle;
		DWORD _vertexFormat;
		D3DCOLOR _tintColor;
		D3DCOLOR _transparentColor;
		D3DCOLOR _backgroundFillColor;
		
	public:
		DXRenderer(HWND hwnd);
		virtual ~DXRenderer();

		ITextureCache& getTextureCache();

		

		Util::Color getTransparentColor() const;
		void setTransparentColor(const Util::Color& color);
		void setTransparentColor(uint8_t red, uint8_t green, uint8_t blue);
		
		

		

		
		DXFont LoadFont(const char* name, uint32_t size, uint32_t weight, bool italic) const;

		uint32_t MakeFont(const char* name, uint32_t size, uint32_t weight, bool italic);
		uint32_t getFontHandle(const char* filename);
		const FontInfo& getFontInfo(uint32_t handle) const;

		bool BeginSpriteBatch(bool alphablend);
		void EndSpriteBatch();
		
		void setTransform2D(const Util::Vec2* translation, const Util::Vec2* scalingCenter, const Util::Vec2* scale, const Util::Vec2* rotationPivot, float rotationRadians, const Util::Color* colorTint);
		void setTransform3D(const Util::Vec3* translation, const Util::Vec3* scalingCenter, const Util::Vec3* scale, const Util::Vec3* rotationPivot, const Util::Vec3* rotationRadians, const Util::Color* colorTint);
		void setFontStyle(bool noClip, bool singleLine, bool hCenter, bool right, bool vCenter, bool bottom);

		void DrawTexture(const TextureData& texture);
		void DrawSprite(const TextureData& texture, const SpriteInfo& sprite);
		void DrawFont(uint32_t hFont, const char* text, const Util::Rect* bounds);
		void DrawPoint(const Util::Vec2& pos, const Util::Color* color, float lineWidth);
		void DrawLine(const Util::Vec2& start, const Util::Vec2& finish, const Util::Color* color, float lineWidth);
		void DrawTriangle(const Util::Vec2& p1, const Util::Vec2& p2, const Util::Vec2& p3, const Util::Color* color, float lineWidth);
		void DrawRectangle(const Util::Vec2& pos, uint32_t hwidth, uint32_t hheight, const Util::Color* color, float lineWidth);
		void DrawRectangle(const Util::Rect& rect, const Util::Color* color, float lineWidth);
		void DrawCircle(const Util::Vec2& pos, float radius, const Util::Color* color, float lineWidth);
		void DrawElipse(const Util::Vec2& pos, float xRadius, float yRadius, const Util::Color* color, float lineWidth);

		void DrawPrimitive(const Vertex* buffer, uint32_t numVertices, DP_Type type, uint32_t numPrimitives);
		void DrawIndexedPrimitive(const Vertex* vertexBuffer, uint32_t numVertices, const uint16_t* indexBuffer, uint32_t numIndices, DP_Type drawType, uint32_t numPrimitives);




		//new code, remove the old code after everything works
		bool setTranslation(const Util::Vec3& translation);
		bool setScaling(const Util::Vec3& scale);
		bool setRotation(const Util::Vec3& rotation);
		bool setWorldTransformMatrix();
		bool setProjectionMatrix(float vertFovDeg, float nearViewPlane, float farViewPlane, float screenAspectRatio);
		bool setViewMatrix(const Util::Vec3& cameraPosition, const Util::Vec3& cameraLookAt, const Util::Vec3& cameraUpVector);
		bool appendFVF(FVF_Type format);
		bool clearFVF();
		bool applyFVF(uint32_t format);
		uint32_t getFVF() const;
		bool setRenderStateLighting(bool enabled);
		bool setRenderStateZBuffer(bool enabled);
		bool setRenderStateCulling(RS_Culling cullmode);
		bool setRenderStateFillmode(RS_Fillmode fillmode);
		*/

	};
}
