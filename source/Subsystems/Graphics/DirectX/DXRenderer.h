#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <deque>
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/SpriteInfo.h"
#include "Subsystems/Graphics/IRenderer.h"
#include "Subsystems/Graphics/DirectX/DXFontCache.h"
#include "Subsystems/Graphics/DirectX/DXInclude.h"
#include "Subsystems/Graphics/DirectX/DXTextureCache.h"
#include "Util/Dimensional.h"
#include "Util/Color.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class DXRenderer : public IRenderer
	{
	private:
		DXRenderer(DXRenderer& dxg);
		DXRenderer& operator=(DXRenderer& dxg);

		bool InitDevices();
		bool ResetDevices();
		
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
		~DXRenderer();

		ITextureCache& getTextureCache();

		Util::Vec2 getScreenSize() const;
		void setScreenSize(const Util::Vec2& size);
		void setScreenSize(uint32_t width, uint32_t height);

		Util::Color getTransparentColor() const;
		void setTransparentColor(const Util::Color& color);
		void setTransparentColor(uint8_t red, uint8_t green, uint8_t blue);
		
		Util::Color getBackgroundFillColor() const;
		void setBackgroundFillColor(const Util::Color& color);
		void setBackgroundFillColor(uint8_t red, uint8_t green, uint8_t blue);

		bool getFullscreenState() const;
		void setFullscreenState(bool state);

		
		DXFont LoadFont(const char* name, uint32_t size, uint32_t weight, bool italic) const;

		uint32_t MakeFont(const char* name, uint32_t size, uint32_t weight, bool italic);
		uint32_t getFontHandle(const char* filename);
		const FontInfo& getFontInfo(uint32_t handle) const;

		bool BeginScene();
		void EndScene();
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
	};
}