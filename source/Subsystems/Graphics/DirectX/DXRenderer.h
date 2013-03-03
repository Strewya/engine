#ifndef GRAPHICS_DXRENDERER_H_
#define GRAPHICS_DXRENDERER_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <deque>
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/SpriteInfo.h"
#include "Subsystems/Graphics/Interface.h"
#include "Subsystems/Graphics/DirectX/DXFontCache.h"
#include "Subsystems/Graphics/DirectX/DXInclude.h"
#include "Util/Dimensional.h"
#include "Util/Color.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class DXRenderer : public Interface
	{
	private:
		DXRenderer(DXRenderer& dxg);
		DXRenderer& operator=(DXRenderer& dxg);

		bool InitDevices();
		bool ResetDevices();
		
		RECT MakeRECT(const Util::Rect& in) const;
		D3DXVECTOR2 MakeVECTOR(const Util::Vec2& in) const;
		D3DXVECTOR3 MakeVECTOR(const Util::Vec3& in) const;
		D3DCOLOR MakeCOLOR(const Util::Color& in) const;

		HWND _hwnd;
		LPDIRECT3D9 _d3d;
		LPDIRECT3DDEVICE9 _d3ddev;
		LPDIRECT3DSURFACE9 _backbuffer;
		LPD3DXSPRITE _spriteHandler;
		LPD3DXLINE _line;
		
		D3DPRESENT_PARAMETERS _d3dpp;
		
		struct dxtexture
		{
			LPDIRECT3DTEXTURE9 texture;
			uint refs;
		};
		std::deque<dxtexture> _textures;
		std::set<uint> _freeTextureSlots;
		DXFontCache _fonts;

		D3DXMATRIX _transformMatrix;
		DWORD _fontStyle;
		D3DCOLOR _tintColor;
		D3DCOLOR _transparentColor;
		D3DCOLOR _backgroundFillColor;
		
	public:
		DXRenderer(HWND hwnd);
		~DXRenderer();
		Util::Vec2 getScreenSize() const;
		void setScreenSize(const Util::Vec2& size);
		void setScreenSize(uint width, uint height);

		Util::Color getTransparentColor() const;
		void setTransparentColor(const Util::Color& color);
		void setTransparentColor(uint red, uint green, uint blue);
		
		Util::Color getBackgroundFillColor() const;
		void setBackgroundFillColor(const Util::Color& color);
		void setBackgroundFillColor(uint red, uint green, uint blue);

		bool getFullscreenState() const;
		void setFullscreenState(bool state);

		
		DXFont LoadFont(const char* name, uint size, uint weight, bool italic) const;

		Texture LoadTexture(const char* filename);
		void LoadTexture(uint index);
		void ReleaseTexture(uint handle);

		uint MakeFont(const char* name, uint size, uint weight, bool italic);
		uint getFontHandle(const char* filename);
		const FontInfo& getFontInfo(uint handle) const;

		bool BeginScene();
		void EndScene();
		bool BeginSpriteBatch(bool alphablend);
		void EndSpriteBatch();
		
		void setTransform2D(const Util::Vec2* translation, const Util::Vec2* scalingCenter, const Util::Vec2* scale, const Util::Vec2* rotationPivot, float rotationRadians, const Util::Color* colorTint);
		void setFontStyle(bool noClip, bool singleLine, bool hCenter, bool right, bool vCenter, bool bottom);

		void DrawTexture(const Texture& texture);
		void DrawSprite(const Texture& texture, const SpriteInfo& sprite);
		void DrawFont(uint hFont, const char* text, const Util::Rect* bounds);
		void DrawPoint(const Util::Vec2& pos, const Util::Color* color, float lineWidth);
		void DrawLine(const Util::Vec2& start, const Util::Vec2& finish, const Util::Color* color, float lineWidth);
		void DrawTriangle(const Util::Vec2& p1, const Util::Vec2& p2, const Util::Vec2& p3, const Util::Color* color, float lineWidth);
		void DrawRectangle(const Util::Vec2& pos, uint hwidth, uint hheight, const Util::Color* color, float lineWidth);
		void DrawRectangle(const Util::Rect& rect, const Util::Color* color, float lineWidth);
		void DrawCircle(const Util::Vec2& pos, float radius, const Util::Color* color, float lineWidth);
		void DrawElipse(const Util::Vec2& pos, float xRadius, float yRadius, const Util::Color* color, float lineWidth);
	};

	inline RECT DXRenderer::MakeRECT(const Util::Rect& in) const
	{
		RECT out = {(int)in.Left(), (int)in.Top(), (int)in.Right(), (int)in.Bottom()} ;
		return out;
	}

	inline D3DXVECTOR2 DXRenderer::MakeVECTOR(const Util::Vec2& in) const
	{
		return D3DXVECTOR2(in.x, in.y);
	}

	inline D3DXVECTOR3 DXRenderer::MakeVECTOR(const Util::Vec3& in) const
	{
		return D3DXVECTOR3(in.x, in.y, in.z);
	}

	inline D3DCOLOR DXRenderer::MakeCOLOR(const Util::Color& in) const
	{
		return D3DCOLOR_ARGB(in.alpha, in.red, in.green, in.blue);
	}



	//Store<DXTexture, TextureDescriptor, DXRenderer> TextureStore;
}

#endif //GRAPHICS_DXRENDERER_H_