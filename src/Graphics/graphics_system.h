#pragma once
/********************************************
*  contents:   main graphics system interface
*  usage:
********************************************/
/******* c++ headers *******/
#include <cassert>
#include <vector>
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
#include "graphics/dx_renderer.h"
#include "graphics/graphics_types.h"
#include "graphics/shader/pixel/pixel_shader_manager.h"
#include "graphics/shader/vertex/vertex_shader_manager.h"
#include "graphics/texture/texture_manager.h"
#include "util/geometry/vec2.h"
#include "window/window_proxy.h"
/******* end header inclusion *******/

namespace Core
{
   struct Camera;
   struct Color;
   struct Mesh;
   struct Transform;
   struct Vertex;


   struct GraphicsSystem
   {
   public:
      bool init(WindowProxy window);
      bool shutdown();

      void begin();
      void present();

      void setOrthographicProjection();
      void setPerspectiveProjection();
      void applyCamera(const Camera& camera);
      void clearCamera();

      void setCulling(bool isEnabled);
      void setTransparency(bool isEnabled);

      void renderMesh(Transform t, Color c, const Mesh& mesh);

      TextureManager textures;
      VertexShaderManager vertexShaders;
      PixelShaderManager pixelShaders;
      //DXBufferManager m_buffers; //predefined mesh buffers?
      
      //#todo make it private later when all testing is done
      DXRenderer _renderer;
   private:
      template<typename T> void declare(T*& ptr);
      bool initDevice();
      bool initSwapChain();
      bool initRenderTarget();
      bool initViewport();
      bool initSamplerState();
      bool initDepthBuffer();

      TextureFileLoader m_textureFileLoader;
      VertexShaderLoader m_vsLoader;
      VertexShaderFileLoader m_vsFileLoader;
      PixelShaderLoader m_psLoader;
      PixelShaderFileLoader m_psFileLoader;

      D3DXCOLOR m_backgroundColor;
      vec2i m_backbufferSize;

      std::vector<IUnknown**> m_declaredObjects;
      
      WindowProxy m_window;

      IDXGIFactory* m_dxgiFactory;
      ID3D11Device* m_dev;
      ID3D11DeviceContext* m_devcon;
      IDXGISwapChain* m_swapchain;
      ID3D11RenderTargetView* m_renderTarget;
      ID3D11SamplerState* m_samplerState;
      ID3D11Texture2D* m_depthStencilBuffer;
      ID3D11DepthStencilView* m_depthStencilView;
      ID3D11BlendState* m_transparency;
      ID3D11RasterizerState* m_cullingEnabled;
      ID3D11RasterizerState* m_cullingDisabled;
   };

   template<typename T> void GraphicsSystem::declare(T*& ptr)
   {
      ptr = nullptr;
      m_declaredObjects.emplace_back((IUnknown**)&ptr);
   }
}