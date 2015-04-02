#pragma once
/********************************************
*  class:   GraphicsSystem
*  usage:
********************************************/
/******* C++ headers *******/
#include <cassert>
#include <vector>
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
#include <Graphics/DXRenderer.h>
#include <Graphics/Shader/DXShaderManager.h>
#include <Graphics/Texture/DXTextureManager.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   class Camera;
   class Vertex;
   class Window;

   

   class GraphicsSystem
   {
   public:
      bool init(Window& window, Vec2i backbufferSize);
      bool shutdown();

      void begin();
      void present();

      void setOrthographicProjection();
      void setPerspectiveProjection();
      void applyCamera(const Camera& camera);
      void clearCamera();

      

      DXRenderer m_renderer;
      DXTextureManager m_textures;
      DXShaderManager m_shaders;
      //DXBufferManager m_buffers; //predefined mesh buffers?

   private:
      template<typename T> void declare(T*& ptr);
      bool initDevice();
      bool initSwapChain();
      bool initRenderTarget();
      bool initViewport();
      bool initSamplerState();
      bool initDepthBuffer();

      ID3D11InputLayout* createInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc);

      Vec2i m_backbufferSize;
      Window* m_window;

      //DX specific
      D3DXCOLOR m_backgroundColor;
      XMMATRIX m_camView;
      XMMATRIX m_camProjection;

      IDXGIFactory* m_dxgiFactory;
      ID3D11Device* m_dev;
      ID3D11DeviceContext* m_devcon;
      IDXGISwapChain* m_swapchain;
      ID3D11RenderTargetView* m_renderTarget;
      ID3D11SamplerState* m_samplerState;
      ID3D11Texture2D* m_depthStencilBuffer;
      ID3D11DepthStencilView* m_depthStencilView;

      std::vector<IUnknown**> m_declaredObjects;
   };

   template<typename T> void GraphicsSystem::declare(T*& ptr)
   {
      ptr = nullptr;
      m_declaredObjects.emplace_back((IUnknown**)&ptr);
   }
}