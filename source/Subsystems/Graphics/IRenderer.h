#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
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
	class TextureData;
	class ITextureCache;
	struct Vertex;
}

namespace Graphics
{
	enum class RS_Culling
	{
		None,
		Clockwise,
		CounterClockwise,
	};

	enum class RS_Fillmode
	{
		Point,
		Wireframe,
		Solid,
	};

	enum class DP_Type
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,
		TriangleFan,
	};

	enum class FVF_Type
	{
		XYZ,
		XYZRHW,
		NORMAL,
		DIFFUSE,
		SPECULAR,
		TEX0,
		TEX1,
		TEX2,
		TEX3,
		TEX4,
		TEX5,
		TEX6,
		TEX7,
		TEX8,
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer() {}

		virtual ITextureCache& getTextureCache() = 0;

		virtual Util::Vec2 getScreenSize() const = 0;
		virtual void setScreenSize(const Util::Vec2& size) = 0;
		virtual void setScreenSize(uint32_t width, uint32_t height) = 0;
		
		virtual Util::Color getTransparentColor() const = 0;
		virtual void setTransparentColor(const Util::Color& color) = 0;
		virtual void setTransparentColor(uint8_t red, uint8_t green, uint8_t blue) = 0;
		
		virtual Util::Color getBackgroundFillColor() const = 0;
		virtual void setBackgroundFillColor(const Util::Color& color) = 0;
		virtual void setBackgroundFillColor(uint8_t red, uint8_t green, uint8_t blue) = 0;

		virtual bool getFullscreenState() const = 0;
		virtual void setFullscreenState(bool state) = 0;
		
		virtual bool BeginScene() = 0;
		virtual void EndScene() = 0;
		virtual bool BeginSpriteBatch(bool alphablend) = 0;
		virtual void EndSpriteBatch() = 0;

		virtual void setTransform2D(const Util::Vec2* translation, const Util::Vec2* scalingCenter, const Util::Vec2* scale, const Util::Vec2* rotationPivot, float rotationRadians, const Util::Color* colorTint) = 0;
		virtual void setTransform3D(const Util::Vec3* translation, const Util::Vec3* scalingCenter, const Util::Vec3* scale, const Util::Vec3* rotationPivot, const Util::Vec3* rotationRadians, const Util::Color* colorTint) = 0;
		virtual void setFontStyle(bool noClip, bool singleLine, bool hCenter, bool right, bool vCenter, bool bottom) = 0;
		
		virtual uint32_t MakeFont(const char* name, uint32_t size, uint32_t weight, bool italic) = 0;
		virtual uint32_t getFontHandle(const char* filename) = 0;
		virtual const FontInfo& getFontInfo(uint32_t handle) const = 0;


		virtual void DrawTexture(const TextureData& texture) = 0;
		virtual void DrawSprite(const TextureData& texture, const SpriteInfo& sprite) = 0;
		virtual void DrawFont(uint32_t hFont, const char* text, const Util::Rect* bounds) = 0;
		
		virtual void DrawPoint(const Util::Vec2& pos, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawLine(const Util::Vec2& start, const Util::Vec2& finish, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawTriangle(const Util::Vec2& p1, const Util::Vec2& p2, const Util::Vec2& p3, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawRectangle(const Util::Vec2& pos, uint32_t hwidth, uint32_t hheight, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawRectangle(const Util::Rect& rect, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawCircle(const Util::Vec2& pos, float radius, const Util::Color* color = nullptr, float lineWidth = 1) = 0;
		virtual void DrawElipse(const Util::Vec2& pos, float xRadius, float yRadius, const Util::Color* color = nullptr, float lineWidth = 1) = 0;

		virtual void DrawPrimitive(const Vertex* buffer, uint32_t numVertices, DP_Type type, uint32_t numPrimitives) = 0;
		virtual void DrawIndexedPrimitive(const Vertex* vertexBuffer, uint32_t numVertices, const uint16_t* indexBuffer, uint32_t numIndices, DP_Type drawType, uint32_t numPrimitives) = 0;
		
		virtual bool setTranslation(const Util::Vec3& translation) = 0;
		virtual bool setScaling(const Util::Vec3& scale) = 0;
		virtual bool setRotation(const Util::Vec3& rotation) = 0;
		virtual bool setWorldTransformMatrix() = 0;
		virtual bool setProjectionMatrix(float vertFovDeg, float nearViewPlane, float farViewPlane, float screenAspectRatio) = 0;
		virtual bool setViewMatrix(const Util::Vec3& cameraPosition, const Util::Vec3& cameraLookAt, const Util::Vec3& cameraUpVector) = 0;
		virtual bool appendFVF(FVF_Type format) = 0;
		virtual bool clearFVF() = 0;
		virtual bool applyFVF(uint32_t format) = 0;
		virtual uint32_t getFVF() const = 0;
		virtual bool setRenderStateLighting(bool enabled) = 0;
		virtual bool setRenderStateZBuffer(bool enabled) = 0;
		virtual bool setRenderStateCulling(RS_Culling cullmode) = 0;
		virtual bool setRenderStateFillmode(RS_Fillmode fillmode) = 0;

	};
}
