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

      void togglePixelShader();

      Vec2f getTextureDimensions(const Texture& texture) const;
      void setBackgroundColor(float red, float green, float blue);

      void setCulling(bool isEnabled);
      void setTransparencyMode(bool isEnabled);

      void setOrthographicProjection();
      void setPerspectiveProjection();

      void applyCamera(const Camera& camera);
      void clearCamera();

      Vec2f screenToWorld(Vec2f screen, Camera& camera) const;
      Vec2f worldToScreen(Vec2f world, Camera& camera) const;

      Vec2f textHalfSize(uint32_t fontID, const std::string& text) const;
      Transform justifyText(Transform textTf, float textSizeX, TextJustification justification);

      bool loadTexture(const ResourceFile& filename, Texture& outTexture);
      bool reloadTexture(const ResourceFile& filename, Texture& outTexture);
      void unloadTexture(Texture& texture);

      std::vector<Vertex> v3_makeCustomVertices(const Vec2f* pos, uint32_t pts) const;
      std::vector<Vertex> v3_makeCircleVertices(const Vec2f& pos, float radius, uint32_t pts) const;
      std::vector<Vertex> v3_makeQuadVertices(Vec2f pos, Vec2f halfSize) const;
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
      void v3_setInstanceData(const std::vector<Transform>& tfs, const std::vector<Color>& fills, const std::vector<float>& healths);
      void v3_setTexture(uint32_t textureId);
      void v3_setFontTexture(uint32_t fontId);
      void v3_draw(uint32_t indiceCount, uint32_t instanceCount);

      void v3_setTextureAsRenderTarget();
      void v3_clearTextureAsRenderTarget();
      void v3_setTextureFromRenderTarget();

      void createTextureRenderTarget(uint32_t width, uint32_t height);
      void clearTextureRenderTarget();

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
      ID3D11PixelShader* m_healthbarShader;
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
      std::vector<Vec2f> m_circleData;

      //texture render targets
      struct RenderTargetTexture
      {
         ID3D11Texture2D* texture;
         ID3D11RenderTargetView* rtView;
         ID3D11ShaderResourceView* srView;
      };
      RenderTargetTexture m_RTtexture;
   };

   template<typename T> void GraphicsSystem::safeRelease(T*& ptr)
   {
      if( ptr != nullptr )
      {
         releasePtr(ptr);
         ptr = nullptr;
      }
   }

   template<typename T> void GraphicsSystem::releasePtr(T* ptr)
   {
      if( ptr != nullptr )
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

   void drawLine(GraphicsSystem& gfx, Vec2f p1, Vec2f p2, Transform transform, Color color);
   void drawLine(GraphicsSystem& gfx, Vec2f p1, Color c1, Vec2f p2, Color c2, Transform transform, Color color);
   void drawMultipleLines(GraphicsSystem& gfx, Vec2f p1, Vec2f p2,
                          const std::vector<Transform>& transforms,
                          const std::vector<Color>& colors,
                          const std::vector<float>& healths);

   void drawMultiline(GraphicsSystem& gfx, const Vec2f* positions, uint32_t count, Transform transform, Color color);

   void drawHollowPolygon(GraphicsSystem& gfx, const Vec2f* positions, uint32_t count, Transform transform, Color color);
   void drawMultipleHollowPolygons(GraphicsSystem& gfx, const Vec2f* positions, uint32_t count,
                                   const std::vector<Transform>& transforms,
                                   const std::vector<Color>& colors,
                                   const std::vector<float>& healths);

   void drawSolidPolygon(GraphicsSystem& gfx, const Vec2f* positions, uint32_t count, Transform transform, Color color);
   void drawMultipleSolidPolygons(GraphicsSystem& gfx, const Vec2f* positions, uint32_t count,
                                  const std::vector<Transform>& transforms,
                                  const std::vector<Color>& colors,
                                  const std::vector<float>& healths);

   void drawHollowQuad(GraphicsSystem& gfx, Vec2f halfSize, Transform transform, Color color);
   void drawMultipleHollowQuads(GraphicsSystem& gfx, Vec2f halfSize,
                                const std::vector<Transform>& transforms,
                                const std::vector<Color>& colors,
                                const std::vector<float>& healths);

   void drawSolidQuad(GraphicsSystem& gfx, Vec2f halfSize, Transform transform, Color color);
   void drawMultipleSolidQuads(GraphicsSystem& gfx, Vec2f halfSize,
                               const std::vector<Transform>& transforms,
                               const std::vector<Color>& colors,
                               const std::vector<float>& healths);

   void drawHollowCircle(GraphicsSystem& gfx, float radius, uint32_t points, Transform transform, Color color);
   void drawMultipleHollowCircles(GraphicsSystem& gfx, float radius, uint32_t points,
                                  const std::vector<Transform>& transforms,
                                  const std::vector<Color>& colors,
                                  const std::vector<float>& healths);

   void drawSolidCircle(GraphicsSystem& gfx, float radius, uint32_t points, Transform transform, Color color);
   void drawMultipleSolidCircles(GraphicsSystem& gfx, float radius, uint32_t points,
                                 const std::vector<Transform>& transforms,
                                 const std::vector<Color>& colors,
                                 const std::vector<float>& healths);

   void drawTexturedQuad(GraphicsSystem& gfx, uint32_t textureID, Rect image, Vec2f halfSize, Transform transform, Color color, float health);
   void drawMultipleTexturedQuads(GraphicsSystem& gfx, uint32_t textureID, Rect image, Vec2f halfSize,
                                  const std::vector<Transform>& transform,
                                  const std::vector<Color>& color,
                                  const std::vector<float>& healths);

   void drawText(GraphicsSystem& gfx, uint32_t fontID, const std::string& text, Transform transform, Color tint, TextJustification justification, bool isItalic);
}