#ifndef GRAPHICS_INTERFACE_H_
#define GRAPHICS_INTERFACE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	class Color;
	class Rect;
	class Vec2;
	class Vec3;
}

namespace Graphics
{
	class FontInfo;
	class SpriteInfo;
	class Spritesheet;
	class Texture;
}

namespace Graphics
{
	class Interface
	{
	public:
		virtual Util::Vec2 getScreenSize() const = 0;
		virtual void setScreenSize(const Util::Vec2& size) = 0;
		virtual void setScreenSize(uint width, uint height) = 0;
		
		virtual Util::Color getTransparentColor() const = 0;
		virtual void setTransparentColor(const Util::Color& color) = 0;
		virtual void setTransparentColor(uint red, uint green, uint blue) = 0;
		
		virtual Util::Color getBackgroundFillColor() const = 0;
		virtual void setBackgroundFillColor(const Util::Color& color) = 0;
		virtual void setBackgroundFillColor(uint red, uint green, uint blue) = 0;

		virtual bool getFullscreenState() const = 0;
		virtual void setFullscreenState(bool state) = 0;
		
		virtual Texture LoadTexture(const char* filename) = 0;
		virtual void LoadTexture(uint index) = 0;
		virtual void ReleaseTexture(uint handle) = 0;

		virtual uint MakeFont(const char* name, uint size, uint weight, bool italic) = 0;
		virtual uint getFontHandle(const char* filename) = 0;
		virtual const FontInfo& getFontInfo(uint handle) const = 0;

		virtual bool BeginScene() = 0;
		virtual void EndScene() = 0;
		virtual bool BeginSpriteBatch(bool alphablend) = 0;
		virtual void EndSpriteBatch() = 0;

		virtual void setTransform2D(const Util::Vec2* translation, const Util::Vec2* scalingCenter, const Util::Vec2* scale, const Util::Vec2* rotationPivot, float rotationRadians, const Util::Color* colorTint) = 0;
		virtual void setFontStyle(bool noClip, bool singleLine, bool hCenter, bool right, bool vCenter, bool bottom) = 0;
		
		virtual void DrawTexture(const Texture& texture) = 0;
		virtual void DrawSprite(const Texture& texture, const SpriteInfo& sprite) = 0;
		virtual void DrawFont(uint hFont, const char* text, const Util::Rect* bounds) = 0;
		
		virtual void DrawPoint(const Util::Vec2& pos, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawLine(const Util::Vec2& start, const Util::Vec2& finish, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawTriangle(const Util::Vec2& p1, const Util::Vec2& p2, const Util::Vec2& p3, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawRectangle(const Util::Vec2& pos, uint hwidth, uint hheight, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawRectangle(const Util::Rect& rect, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawCircle(const Util::Vec2& pos, float radius, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawElipse(const Util::Vec2& pos, float xRadius, float yRadius, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
	};
}

#endif //_GRAPHICS_INTERFACE_H_