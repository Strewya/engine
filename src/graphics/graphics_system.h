#pragma once
/********************************************
*  contents:   main graphics system interface
*  usage:
********************************************/
/******* c++ headers *******/
#include <array>
/******* common headers *******/
#include "graphics/dx_include.h"
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/dx_renderer.h"
#include "graphics/graphics_types.h"
#include "graphics/shader/pixel/pixel_shader_cache.h"
#include "graphics/shader/pixel/pixel_shader_loader.h"
#include "graphics/shader/vertex/vertex_shader_cache.h"
#include "graphics/shader/vertex/vertex_shader_loader.h"
#include "graphics/texture/texture_cache.h"
#include "graphics/texture/texture_file_loader.h"
#include "utility/geometry/rect.h"
/******* end header inclusion *******/

namespace core
{
   struct Camera;
   struct Color;
   struct Mesh;
   struct Rect;
   struct Transform;
   struct Vertex;
   struct FontDescriptor;
   
   enum TextJustification;

   struct GraphicsSystem
   {
   public:
      bool init(LinearAllocator& a, u32 textureSlots, u32 shaderSlots, u64 window, u32 width, u32 height);
      bool shutdown();

      void begin();
      void present();

      void setOrthographicProjection();
      void setPerspectiveProjection();
      void applyCamera(const Camera& camera);
      void clearCamera();
      v2 screenToWorld(const Camera& cam, v2 screenPos);
      v2 worldToScreen(const Camera& cam, v2 worldPos);

      void setCulling(bool isEnabled);
      void setTransparency(bool isEnabled);

      void renderMesh(Transform t, Color c, const Mesh& mesh);

      HTexture loadTextureFromFile(const char* filename);
      HVertexShader loadVertexShaderFromFile(const char* filename, VertexType vType);
      HPixelShader loadPixelShaderFromFile(const char* filename);

      void unload(HTexture handle);
      void unload(HVertexShader handle);
      void unload(HPixelShader handle);
      
      TextureCache textures;
      VertexShaderCache vertexShaders;
      PixelShaderCache pixelShaders;
      //DXBufferManager m_buffers; //predefined mesh buffers?
      
   private:
      template<typename T> void declare(T*& ptr);
      bool initDevice();
      bool initSwapChain();
      bool initRenderTarget();
      bool initViewport();
      bool initSamplerState();
      bool initDepthBuffer();

      StackAllocator m_stackAllocator;
      TextureFileLoader m_textureFileLoader;
      VertexShaderLoader m_vsLoader;
      PixelShaderLoader m_psLoader;
      DXRenderer m_renderer;

      D3DXCOLOR m_backgroundColor;
      v2 m_backbufferSize;

      enum
      {
         InterfaceCount = 11,
      };

      IUnknown** m_declaredObjects[InterfaceCount];
      u32 m_emptyInterfaceSlot;
      
      u64 m_window;
      f32 width;
      f32 height;

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
      CORE_ASSERT_DBGERR(m_emptyInterfaceSlot < InterfaceCount, "Reached maximum available interface declarations!");
      ptr = nullptr;
      m_declaredObjects[m_emptyInterfaceSlot++] = (IUnknown**)&ptr;
   }
}