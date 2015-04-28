#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
#include <cassert>
#include <vector>
/******* common headers *******/
#include <graphics/dx_include.h>
/******* extra headers *******/
#include <graphics/dx_renderer.h>
#include <graphics/graphics_typedefs.h>
#include <graphics/shader/pixel/pixel_shader_manager.h>
#include <graphics/shader/vertex/vertex_shader_manager.h>
#include <graphics/texture/texture_manager.h>
#include <util/vec2.h>
/******* end header inclusion *******/

namespace Core
{
   class Camera;
   class Color;
   class Mesh;
   class Transform;
   class Vertex;
   class Window;



   class GraphicsSystem
   {
   public:
      bool init(Window& window);
      bool shutdown();

      void begin();
      void present();

      void setOrthographicProjection();
      void setPerspectiveProjection();
      void applyCamera(const Camera& camera);
      void clearCamera();

      void renderMesh(Transform t, Color c, const Mesh& mesh);

      DXRenderer renderer;
      DXTextureManager textures;
      VertexShaderManager vertexShaders;
      PixelShaderManager pixelShaders;
      //DXBufferManager m_buffers; //predefined mesh buffers?

   private:
      template<typename T> void declare(T*& ptr);
      bool initDevice();
      bool initSwapChain();
      bool initRenderTarget();
      bool initViewport();
      bool initSamplerState();
      bool initDepthBuffer();

      DXTextureFileLoader m_textureFileLoader;
      VertexShaderLoader m_vsLoader;
      VertexShaderFileLoader m_vsFileLoader;
      PixelShaderLoader m_psLoader;
      PixelShaderFileLoader m_psFileLoader;

      D3DXCOLOR m_backgroundColor;
      Vec2i m_backbufferSize;

      std::vector<IUnknown**> m_declaredObjects;
      
      Window* m_window;

      IDXGIFactory* m_dxgiFactory;
      ID3D11Device* m_dev;
      ID3D11DeviceContext* m_devcon;
      IDXGISwapChain* m_swapchain;
      ID3D11RenderTargetView* m_renderTarget;
      ID3D11SamplerState* m_samplerState;
      ID3D11Texture2D* m_depthStencilBuffer;
      ID3D11DepthStencilView* m_depthStencilView;
   };

   template<typename T> void GraphicsSystem::declare(T*& ptr)
   {
      ptr = nullptr;
      m_declaredObjects.emplace_back((IUnknown**)&ptr);
   }
}