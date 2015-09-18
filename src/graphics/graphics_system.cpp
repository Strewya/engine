//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/graphics_system.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/font/font_descriptor.h"
#include "graphics/mesh/mesh.h"
#include "graphics/shader/vertex/vertex_shader_loader.h"
#include "graphics/shader/pixel/pixel_shader_loader.h"
#include "graphics/camera.h"
#include "graphics/vertex.h"
#include "utility/color.h"
#include "utility/transform.h"
#include "utility/utility.h"
#include "utility/file_loader.h"
#include "utility/geometry/rect.h"
#include "utility/geometry/vec3.h"
#include "window/window.h"
/******* end headers *******/

namespace core
{
   //*****************************************************************
   //          FILE STATIC FUNCTION DECLARATIONS
   //*****************************************************************
   core_internal XMVECTOR convert(v2 v);
   core_internal XMVECTOR convert(v3 v);
   core_internal XMMATRIX calculateCamView(const Camera& cam);

   //*****************************************************************
   //          INIT
   //*****************************************************************
   GraphicsSystem* createGraphicsSystem(MemoryBlock memory)
   {
      LinearAllocator a;
      a.init("Graphics allocator", memory);
      GraphicsSystem* result = allocate<GraphicsSystem>(a);
      result->m_staticMemory = a;
      return result;
   }

   bool GraphicsSystem::init(u32 textureSlots, u32 shaderSlots, u64 handle, u32 width, u32 height)
   {
      CORE_INIT_START(GraphicsSystem);

      m_window = handle;
      m_backbufferSize.x = this->width = (f32)width;
      m_backbufferSize.y = this->height = (f32)height;
      m_backgroundColor.r = m_backgroundColor.g = m_backgroundColor.b = 0;
      clearCamera();

      declare(m_dxgiFactory);
      declare(m_dev);
      declare(m_devcon);
      declare(m_swapchain);
      declare(m_renderTarget);
      declare(m_samplerState);
      declare(m_depthStencilBuffer);
      declare(m_depthStencilView);
      declare(m_transparency);
      declare(m_cullingEnabled);
      declare(m_cullingDisabled);

      CORE_STATUS_AND(initDevice());
      CORE_STATUS_AND(initSwapChain());
      CORE_STATUS_AND(initRenderTarget());
      CORE_STATUS_AND(initViewport());
      CORE_STATUS_AND(initSamplerState());

      CORE_STATUS_AND(m_renderer.init(m_dev, m_devcon, m_samplerState));

      CORE_STATUS_AND(m_textureFileLoader.init(m_dev));
      CORE_STATUS_AND(textures.init(m_staticMemory, textureSlots));

      CORE_STATUS_AND(m_vsLoader.init(m_dev));
      CORE_STATUS_AND(vertexShaders.init(m_staticMemory, shaderSlots));

      CORE_STATUS_AND(m_psLoader.init(m_dev));
      CORE_STATUS_AND(pixelShaders.init(m_staticMemory, shaderSlots));



      if( CORE_STATUS_OK )
      {
#include "graphics/shader/vertex/default_vertex_shader.h"

         auto layout = DefaultVertex::getDescription(m_heapAllocator);
         CORE_ASSERT_DBGERR(layout.buffer != nullptr, "Error generating input layout information");
         auto defaultVertexShader = m_vsLoader.load(layout, (u8*)g_VShader, sizeof(g_VShader));

         deallocate(m_stackAllocator);
         auto vhandle = vertexShaders.insert(defaultVertexShader);
         CORE_ASSERT_DBGERR(vhandle.getIndex() == 0, "The default pixel shader should be at index 0");
         // #todo think about a sane way of using the default shader with different actual vertices being sent to the pipeline
      }

      if( CORE_STATUS_OK )
      {
#include "graphics/shader/pixel/default_pixel_shader.h"

         auto defaultPixelShader = m_psLoader.load((u8*)g_PShader, sizeof(g_PShader));
         auto phandle = pixelShaders.insert(defaultPixelShader);
         CORE_ASSERT_DBGERR(phandle.getIndex() == 0, "The default pixel shader should be at index 0");
      }

      if( CORE_STATUS_OK )
      {
         D3D11_BLEND_DESC blendDesc;
         ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

         blendDesc.AlphaToCoverageEnable = false;

         D3D11_RENDER_TARGET_BLEND_DESC& rtbd = blendDesc.RenderTarget[0];
         rtbd.BlendEnable = true;
         rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
         rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
         rtbd.BlendOp = D3D11_BLEND_OP_ADD;
         rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
         rtbd.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
         rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
         rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

         HRESULT hr = m_dev->CreateBlendState(&blendDesc, &m_transparency);

         CORE_STATUS_AND(SUCCEEDED(hr));

         D3D11_RASTERIZER_DESC rd;
         ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
         rd.FillMode = D3D11_FILL_SOLID;

         rd.CullMode = D3D11_CULL_BACK;
         hr = m_dev->CreateRasterizerState(&rd, &m_cullingEnabled);
         CORE_STATUS_AND(SUCCEEDED(hr));

         rd.CullMode = D3D11_CULL_NONE;
         hr = m_dev->CreateRasterizerState(&rd, &m_cullingDisabled);
         CORE_STATUS_AND(SUCCEEDED(hr));
      }

      CORE_INIT_END;
   }

   //*****************************************************************
   //          SHUTDOWN
   //*****************************************************************
   bool GraphicsSystem::shutdown()
   {
      CORE_SHUTDOWN_START(GraphicsSystem);

      CORE_STATUS_AND(pixelShaders.getCount() == 0);
      CORE_STATUS_AND(vertexShaders.getCount() == 0);
      CORE_STATUS_AND(textures.getCount() == 0);

      CORE_STATUS_AND(m_renderer.shutdown());

      ID3D11Debug* debug = nullptr;
      HRESULT hr = m_dev->QueryInterface(IID_PPV_ARGS(&debug));
      while( m_emptyInterfaceSlot-- )
      {
         safeRelease(*m_declaredObjects[m_emptyInterfaceSlot]);
      }
      if( SUCCEEDED(hr) && debug != nullptr )
      {
         debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
         safeRelease(debug);
      }

      CORE_SHUTDOWN_END;
   }

   //*****************************************************************
   //          LOAD TEXTURE FROM FILE
   //*****************************************************************
   HTexture GraphicsSystem::loadTextureFromFile(const char* filename)
   {
      Texture t = m_textureFileLoader.load(filename);
      HTexture result = textures.insert(t);
      return result;
   }

   //*****************************************************************
   //          UNLOAD TEXTURE
   //*****************************************************************
   void GraphicsSystem::unload(HTexture handle)
   {
      if( !handle.isNull() )
      {
         Texture t = textures.remove(handle);
         m_textureFileLoader.unload(t);
      }
   }

   //*****************************************************************
   //          LOAD VERTEX SHADER FROM FILE
   //*****************************************************************
   HVertexShader GraphicsSystem::loadVertexShaderFromFile(const char* filename, VertexType vType)
   {
      auto file = loadFile(filename, m_stackAllocator);
      CORE_ASSERT_ERR(file.memory != nullptr, "Not enough scratch memory to load shader file '", filename, "'");
      InputLayout layout{};
      switch( vType )
      {
         case VertexType::Default:
         {
            layout = DefaultVertex::getDescription(m_stackAllocator);
         } break;
         case VertexType::Health:
         {
            layout = HealthVertex::getDescription(m_stackAllocator);
         } break;
      }
      CORE_ASSERT_ERR(layout.buffer != nullptr && layout.size > 0, "Error generating input layout information");
      auto shader = m_vsLoader.load(layout, file.memory, file.size);

      deallocate(m_stackAllocator);

      auto result = vertexShaders.insert(shader);
      return result;
   }

   //*****************************************************************
   //          UNLOAD VERTEX SHADER
   //*****************************************************************
   void GraphicsSystem::unload(HVertexShader handle)
   {
      auto shader = vertexShaders.remove(handle);
      m_vsLoader.unload(shader);
   }

   //*****************************************************************
   //          LOAD PIXEL SHADER FROM FILE
   //*****************************************************************
   HPixelShader GraphicsSystem::loadPixelShaderFromFile(const char* filename)
   {
      auto file = loadFile(filename, m_stackAllocator);
      CORE_ASSERT_ERR(file.memory != nullptr, "Not enough scratch memory to load shader file '", filename, "'");
      auto shader = m_psLoader.load(file.memory, file.size);

      deallocate(m_stackAllocator);

      auto result = pixelShaders.insert(shader);
      return result;
   }

   //*****************************************************************
   //          UNLOAD PIXEL SHADER
   //*****************************************************************
   void GraphicsSystem::unload(HPixelShader handle)
   {
      auto shader = pixelShaders.remove(handle);
      m_psLoader.unload(shader);
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
      m_renderer.setProjection(XMMatrixOrthographicLH(width, height, 1.0f, 100.0f));
   }

   //*****************************************************************
   //          SET PERSPECTIVE PROJECTION
   //*****************************************************************
   void GraphicsSystem::setPerspectiveProjection()
   {
      m_renderer.setProjection(XMMatrixPerspectiveFovLH(XMConvertToRadians(45), width / height, 1.0f, 100.0f));
   }

   //*****************************************************************
   //          APPLY CAMERA
   //*****************************************************************
   void GraphicsSystem::applyCamera(const Camera& cam)
   {
      m_renderer.setView(calculateCamView(cam));
   }

   //*****************************************************************
   //          CLEAR CAMERA
   //*****************************************************************
   void GraphicsSystem::clearCamera()
   {
      m_renderer.setView(XMMatrixIdentity());
   }

   //*****************************************************************
   //          WORLD TO SCREEN
   //*****************************************************************
   v2 GraphicsSystem::worldToScreen(const Camera& cam, v2 worldPos)
   {
      auto world = XMMatrixIdentity();
      //world *= XMMatrixScaling(transform.scale.x, transform.scale.y, 1.0f);
      //m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
      //m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
      //world *= XMMatrixRotationZ(transform.rotation);
      world *= XMMatrixTranslation(worldPos.x, worldPos.y, 0);
      world = world*m_renderer.getView()*m_renderer.getProjection();

      XMVECTOR position = XMVector3Project(convert(worldPos), 0, 0, width, height, 0.0f, 1.0f,
                                           m_renderer.getProjection(), calculateCamView(cam), XMMatrixIdentity());

      v2 result{position.m128_f32[0], position.m128_f32[1]};
      return result;
   }

   //*****************************************************************
   //          SCREEN TO WORLD
   //*****************************************************************
   v2 GraphicsSystem::screenToWorld(const Camera& cam, v2 screen)
   {
      auto objectSpace = XMVector3Unproject(convert(v3{screen.x, screen.y, 0.0f}), 0, 0, width, height, 0.0f, 1.0f,
                                            m_renderer.getProjection(), calculateCamView(cam), XMMatrixIdentity());
      auto camPos = convert(cam.getPosition());
      auto plane = XMPlaneFromPoints(convert({0, 0, 0}), convert({1, 0, 0}), convert({0, 1, 0}));
      auto loc = XMPlaneIntersectLine(plane, objectSpace, camPos);
      return v2{loc.m128_f32[0], loc.m128_f32[1]};
   }

   //*****************************************************************
   //          SET CULLING
   //*****************************************************************
   void GraphicsSystem::setCulling(bool isEnabled)
   {
      if( isEnabled )
      {
         m_renderer.setRasterizerState(m_cullingEnabled);
      }
      else
      {
         m_renderer.setRasterizerState(m_cullingDisabled);
      }
   }

   //*****************************************************************
   //          SET TRANSPARENCY
   //*****************************************************************
   void GraphicsSystem::setTransparency(bool isEnabled)
   {
      if( isEnabled )
      {
         m_renderer.setBlendState(m_transparency);
      }
      else
      {
         m_renderer.setBlendState(nullptr);
      }
   }

   //*****************************************************************
   //          RENDER MESH
   //*****************************************************************
   void GraphicsSystem::renderMesh(Transform t, Color c, const Mesh& mesh)
   {
      D3D_PRIMITIVE_TOPOLOGY topology[] = {
         D3D_PRIMITIVE_TOPOLOGY_LINELIST,
         D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
         D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
         D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
      };

      auto vshader = vertexShaders.getData(mesh.vshader);

      m_renderer.setInputLayout(vshader._inputLayout);
      m_renderer.setShader(vshader._vertex);
      m_renderer.setShader(pixelShaders.getData(mesh.pshader)._pixel);
      m_renderer.setTexture(textures.getData(mesh.texture)._shaderResourceView);
      m_renderer.setVertexTopology(topology[mesh.topology]);
      m_renderer.render(t, c, mesh.vertices, mesh.indices);
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
            u32 flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
            flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
            hr = D3D11CreateDevice(adapter, driverType, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &m_dev, nullptr, &m_devcon);

            safeRelease(adapter);
         }
   }

      if( FAILED(hr) )
      {
         CORE_LOG("initDevice failed");
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
      swapChainDesc.OutputWindow = (HWND)m_window;
      swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
      swapChainDesc.Windowed = true;

      DXGI_MODE_DESC& bd = swapChainDesc.BufferDesc;
      bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      bd.Width = (u32)width; //m_backbufferSize.x
      bd.Height = (u32)height; //m_backbufferSize.y
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
         CORE_LOG("initSwapChain failed");
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
         CORE_LOG("initRenderTarget failed");
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
      vp.Width = width;
      vp.Height = height;
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
         CORE_LOG("initSamplerState failed");
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

      dsd.Width = (u32)width; //m_backbufferSize.x;
      dsd.Height = (u32)height; //m_backbufferSize.y;
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
         CORE_LOG("initDepthBuffer failed");
      }
      return SUCCEEDED(hr);
   }

   //*****************************************************************
   //          FILE STATIC FUNCTION DEFINITIONS
   //*****************************************************************
   XMVECTOR convert(v2 v)
   {
      return XMVectorSet(v.x, v.y, 0, 0);
   }

   XMVECTOR convert(v3 v)
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

