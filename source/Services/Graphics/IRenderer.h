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
	class IIndexBuffer;
	class IVertexBuffer;
	struct Vertex;
}

namespace Graphics
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() {}
		
		virtual bool BeginScene() = 0;
		virtual void EndScene() = 0;

		virtual void testDraw() = 0;

		virtual Util::Vec2 getScreenSize() const = 0;
		virtual void setScreenSize(const Util::Vec2& size) = 0;
		virtual void setScreenSize(uint32_t width, uint32_t height) = 0;

		virtual Util::Color getBackgroundColor() const = 0;
		virtual void setBackgroundColor(const Util::Color& color) = 0;
		virtual void setBackgroundColor(float red, float green, float blue) = 0;
		
		virtual bool getFullscreenState() const = 0;
		virtual void setFullscreenState(bool state) = 0;

		virtual uint32_t createVertexBuffer(uint32_t size) = 0;
		virtual IVertexBuffer& getVertexBuffer(uint32_t id) = 0;
		virtual void destroyVertexBuffer(uint32_t id) = 0;

		virtual uint32_t createIndexBuffer(uint32_t size) = 0;
		virtual IIndexBuffer& getIndexBuffer(uint32_t id) = 0;
		virtual void destroyIndexBuffer(uint32_t id) = 0;

		virtual void setIdentity() = 0;
		virtual void setTranslation(const Util::Vec3& translation) = 0;
		virtual void setScaling(const Util::Vec3& scale) = 0;
		virtual void setRotation(const Util::Vec3& rotation) = 0;
		virtual void applyTransform() = 0;
		
		/*
		virtual ITextureCache& getTextureCache() = 0;

		virtual Util::Color getTransparentColor() const = 0;
		virtual void setTransparentColor(const Util::Color& color) = 0;
		virtual void setTransparentColor(uint8_t red, uint8_t green, uint8_t blue) = 0;
		
		
		
		
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
*/
	};
}
