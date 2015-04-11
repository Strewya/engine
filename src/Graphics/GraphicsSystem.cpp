//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/GraphicsSystem.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/Mesh/Mesh.h>
#include <Graphics/Shader/DXShaderLoader.h>
#include <Graphics/Camera.h>
#include <Graphics/Vertex.h>
#include <Util/Color.h>
#include <Util/Transform.h>
#include <Util/Utility.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
   //*****************************************************************
   //          FILE STATIC FUNCTION DECLARATIONS
   //*****************************************************************
   static XMVECTOR convert(Vec2f v);
   static XMVECTOR convert(Vec3f v);
   static XMMATRIX calculateCamView(const Camera& cam);

   //*****************************************************************
   //          INIT
   //*****************************************************************
   bool GraphicsSystem::init(Window& window)
   {
      CORE_INIT_START(GraphicsSystem);

      m_window = &window;
      m_backbufferSize.x = window.getSizeX();
      m_backbufferSize.y = window.getSizeY();
      m_backgroundColor.r = m_backgroundColor.g = m_backgroundColor.b = 0;
      clearCamera();

      //should be declared in reverse order due to how the cleanup is performed
      declare(m_depthStencilView);
      declare(m_depthStencilBuffer);
      declare(m_samplerState);
      declare(m_renderTarget);
      declare(m_swapchain);
      declare(m_devcon);
      declare(m_dev);
      declare(m_dxgiFactory);

      CORE_STATUS_AND(initDevice());
      CORE_STATUS_AND(initSwapChain());
      CORE_STATUS_AND(initRenderTarget());
      CORE_STATUS_AND(initViewport());
      CORE_STATUS_AND(initSamplerState());

      CORE_STATUS_AND(renderer.init(m_dev, m_devcon, m_samplerState));

      DXTextureFileLoader textureFileLoader;
      if( textureFileLoader.init(m_dev) )
      {
         auto defaultTexture = textureFileLoader.load(CORE_RESOURCE("Textures/defaultTexture.png"));
         CORE_STATUS_AND(textures.init(m_dev, defaultTexture));
         textureFileLoader.shutdown();
      }

      DXShaderLoader shaderLoader;
      if( shaderLoader.init(m_dev) )
      {
         DXVertexShader defaultVertexShader{nullptr, nullptr};
         {
#include <Graphics/Shader/defaultVertexShader.h>

            defaultVertexShader = shaderLoader.loadVertexShader(DefaultVertex::getDescription(), (const char*)g_VShader, sizeof(g_VShader));
         }

         DXPixelShader defaultPixelShader{nullptr};
         {
#include <Graphics/Shader/defaultPixelShader.h>

            defaultPixelShader = shaderLoader.loadPixelShader((const char*)g_PShader, sizeof(g_PShader));
         }
         shaderLoader.shutdown();

         CORE_STATUS_AND(shaders.init(m_dev, defaultVertexShader, defaultPixelShader));
      }

      CORE_INIT_END(GraphicsSystem);
   }

   //*****************************************************************
   //          SHUTDOWN
   //*****************************************************************
   bool GraphicsSystem::shutdown()
   {
      CORE_SHUTDOWN_START(GraphicsSystem);

      CORE_STATUS_AND(shaders.shutdown());
      CORE_STATUS_AND(textures.shutdown());
      CORE_STATUS_AND(renderer.shutdown());
      ID3D11Debug* debug = nullptr;
      HRESULT hr = m_dev->QueryInterface(IID_PPV_ARGS(&debug));
      for( auto** unknown : m_declaredObjects )
      {
         safeRelease(*unknown);
      }
      if( SUCCEEDED(hr) && debug != nullptr )
      {
         debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
         safeRelease(debug);
      }

      CORE_SHUTDOWN_END(GraphicsSystem);
   }

   //*****************************************************************
   //          BEGIN
   //*****************************************************************
   void GraphicsSystem::begin()
   {
      m_devcon->ClearRenderTargetView(m_renderTarget, m_backgroundColor);
   }

   //*****************************************************************
   //          PRESENT
   //*****************************************************************
   void GraphicsSystem::present()
   {
      m_swapchain->Present(0, 0);
   }

   //*****************************************************************
   //          SET ORTHOGRAPHIC PROJECTION
   //*****************************************************************
   void GraphicsSystem::setOrthographicProjection()
   {
      renderer.setProjection(XMMatrixOrthographicLH((float)m_window->getSizeX(), (float)m_window->getSizeY(), 1.0f, 100.0f));
   }

   //*****************************************************************
   //          SET PERSPECTIVE PROJECTION
   //*****************************************************************
   void GraphicsSystem::setPerspectiveProjection()
   {
      renderer.setProjection(XMMatrixPerspectiveFovLH(XMConvertToRadians(45), (float)m_window->getSizeX() / m_window->getSizeY(), 1.0f, 100.0f));
   }

   //*****************************************************************
   //          APPLY CAMERA
   //*****************************************************************
   void GraphicsSystem::applyCamera(const Camera& cam)
   {
      renderer.setView(calculateCamView(cam));
   }

   //*****************************************************************
   //          CLEAR CAMERA
   //*****************************************************************
   void GraphicsSystem::clearCamera()
   {
      renderer.setView(XMMatrixIdentity());
   }

   //*****************************************************************
   //          RENDER MESH
   //*****************************************************************
   void GraphicsSystem::renderMesh(Transform t, Color c, const Mesh& mesh)
   {
      renderer.setShader(shaders.getData(mesh.vshader));
      renderer.setShader(shaders.getData(mesh.pshader));
      renderer.setTexture(textures.getData(mesh.texture));
      renderer.setVertexTopology(mesh.topology);
      renderer.render(t, c, mesh.vertices, mesh.indices);
   }

   //*****************************************************************
   //          INIT DEVICE
   //*****************************************************************
   bool GraphicsSystem::initDevice()
   {
      HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);

      if( SUCCEEDED(hr) )
      {
         IDXGIAdapter* adapter = nullptr;
         hr = m_dxgiFactory->EnumAdapters(0, &adapter);
         if( SUCCEEDED(hr) )
         {
            auto driverType = D3D_DRIVER_TYPE_UNKNOWN;
            uint32_t flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
            flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
            hr = D3D11CreateDevice(adapter, driverType, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &m_dev, nullptr, &m_devcon);

            safeRelease(adapter);
         }
      }

      if( FAILED(hr) )
      {
         CORE_INFO("initDevice failed");
      }
      return SUCCEEDED(hr);
   }

   //*****************************************************************
   //          INIT SWAP CHAIN
   //*****************************************************************
   bool GraphicsSystem::initSwapChain()
   {
      DXGI_SWAP_CHAIN_DESC swapChainDesc;
      ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
      swapChainDesc.BufferCount = 1;
      swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
      swapChainDesc.OutputWindow = m_window->getWindowHandle();
      swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
      swapChainDesc.Windowed = true;

      DXGI_MODE_DESC& bd = swapChainDesc.BufferDesc;
      bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      bd.Width = m_window->getSizeX(); //m_backbufferSize.x
      bd.Height = m_window->getSizeY(); //m_backbufferSize.y
      bd.RefreshRate.Denominator = 1;
      bd.RefreshRate.Numerator = 60;
      bd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
      bd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

      DXGI_SAMPLE_DESC& sd = swapChainDesc.SampleDesc;
      sd.Count = 1;
      sd.Quality = 0;

      safeRelease(m_swapchain);
      HRESULT hr = m_dxgiFactory->CreateSwapChain(m_dev, &swapChainDesc, &m_swapchain);
      if( FAILED(hr) )
      {
         CORE_INFO("initSwapChain failed");
      }
      return SUCCEEDED(hr);
   }

   //*****************************************************************
   //          INIT RENDER TARGET
   //*****************************************************************
   bool GraphicsSystem::initRenderTarget()
   {
      ID3D11Texture2D* bbTexture = nullptr;
      HRESULT hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&bbTexture);
      if( SUCCEEDED(hr) )
      {
         safeRelease(m_renderTarget);
         hr = m_dev->CreateRenderTargetView(bbTexture, nullptr, &m_renderTarget);
         if( SUCCEEDED(hr) )
         {
            m_devcon->OMSetRenderTargets(1, &m_renderTarget, nullptr);
         }
         safeRelease(bbTexture);
      }

      if( FAILED(hr) )
      {
         CORE_INFO("initRenderTarget failed");
      }
      return SUCCEEDED(hr);
   }

   //*****************************************************************
   //          INIT VIEWPORT
   //*****************************************************************
   bool GraphicsSystem::initViewport()
   {
      D3D11_VIEWPORT vp;
      ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
      vp.TopLeftX = 0;
      vp.TopLeftY = 0;
      vp.Width = (float)m_window->getSizeX();
      vp.Height = (float)m_window->getSizeY();
      vp.MinDepth = 0;
      vp.MaxDepth = 1;

      m_devcon->RSSetViewports(1, &vp);

      return true;
   }

   //*****************************************************************
   //          INIT SAMPLER STATE
   //*****************************************************************
   bool GraphicsSystem::initSamplerState()
   {
      D3D11_SAMPLER_DESC sampd;
      ZeroMemory(&sampd, sizeof(D3D11_SAMPLER_DESC));
      sampd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      sampd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
      sampd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
      sampd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
      sampd.ComparisonFunc = D3D11_COMPARISON_NEVER;
      sampd.MinLOD = 0;
      sampd.MaxLOD = D3D11_FLOAT32_MAX;

      HRESULT hr = m_dev->CreateSamplerState(&sampd, &m_samplerState);

      if( FAILED(hr) )
      {
         CORE_INFO("initSamplerState failed");
      }
      return SUCCEEDED(hr);
   }

   //*****************************************************************
   //          INIT DEPTH BUFFER
   //*****************************************************************
   bool GraphicsSystem::initDepthBuffer()
   {
      D3D11_TEXTURE2D_DESC dsd;
      ZeroMemory(&dsd, sizeof(D3D11_TEXTURE2D_DESC));

      dsd.Width = m_window->getSizeX(); //m_backbufferSize.x;
      dsd.Height = m_window->getSizeY(); //m_backbufferSize.y;
      dsd.MipLevels = 1;
      dsd.ArraySize = 1;
      dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      dsd.Usage = D3D11_USAGE_DEFAULT;
      dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
      dsd.CPUAccessFlags = 0;
      dsd.MiscFlags = 0;

      DXGI_SAMPLE_DESC& sd = dsd.SampleDesc;
      sd.Count = 1;
      sd.Quality = 0;

      safeRelease(m_depthStencilBuffer);
      auto hr = m_dev->CreateTexture2D(&dsd, nullptr, &m_depthStencilBuffer);
      if( SUCCEEDED(hr) )
      {
         safeRelease(m_depthStencilView);
         hr = m_dev->CreateDepthStencilView(m_depthStencilBuffer, nullptr, &m_depthStencilView);
      }
      if( FAILED(hr) )
      {
         CORE_INFO("initDepthBuffer failed");
      }
      return SUCCEEDED(hr);
   }

   //*****************************************************************
   //          FILE STATIC FUNCTION DEFINITIONS
   //*****************************************************************
   XMVECTOR convert(Vec2f v)
   {
      return XMVectorSet(v.x, v.y, 0, 0);
   }

   XMVECTOR convert(Vec3f v)
   {
      return XMVectorSet(v.x, v.y, v.z, 0);
   }

   XMMATRIX calculateCamView(const Camera& cam)
   {
      auto pos = convert(cam.getPosition());
      auto lookAt = convert(cam.getLookAtAxis());
      auto up = convert(cam.getUpAxis());
      auto rot = convert(cam.getRotation()*cam.getSpeed());

      auto rotMat = XMMatrixRotationRollPitchYawFromVector(rot);
      rot = XMVector3Transform(lookAt, rotMat);
      lookAt = rot + pos;

      return XMMatrixLookAtLH(pos, lookAt, up);
   }
}
