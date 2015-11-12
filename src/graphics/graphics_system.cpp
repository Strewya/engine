//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/graphics_system.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/font/font_descriptor.h"
#include "graphics/texture/texture.h"
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
   struct TextureCache
   {
      Texture* m_buffer;
      u32 m_maxSlots;
      u32 m_usedSlots;
   };

   struct GraphicsSystem
   {
      TextureCache textures;
      VertexShaderCache vertexShaders;
      PixelShaderCache pixelShaders;
      FontCache fonts;

      Memory m_staticMemory;
      Memory m_dynamicMemory;
      Memory m_staticVertexMemory;
      Memory m_dynamicVertexMemory;
      Memory m_staticIndexMemory;
      Memory m_dynamicIndexMemory;
      TextureFileLoader m_textureFileLoader;
      VertexShaderLoader m_vsLoader;
      PixelShaderLoader m_psLoader;
      DXRenderer m_renderer;

      D3DXCOLOR m_backgroundColor;
      v2 m_backbufferSize;
      HPixelShader m_defaultPixelShaderHandle;
      HVertexShader m_defaultVertexShaderHandle;

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

   core_internal bool initDevice(GraphicsSystem* gfx);
   core_internal bool initSwapChain(GraphicsSystem* gfx);
   core_internal bool initRenderTarget(GraphicsSystem* gfx);
   core_internal bool initViewport(GraphicsSystem* gfx);
   core_internal bool initSamplerState(GraphicsSystem* gfx);
   core_internal bool initDepthBuffer(GraphicsSystem* gfx);

   //*****************************************************************
   //          FILE STATIC FUNCTION DECLARATIONS
   //*****************************************************************
   core_internal XMVECTOR convert(v2 v);
   core_internal XMVECTOR convert(v3 v);
   core_internal XMMATRIX calculateCamView(const Camera& cam);



   //*****************************************************************
   //          INIT
   //*****************************************************************
   namespace graphics
   {
      GraphicsSystem* init(Memory mem, u32 textureSlots, u32 shaderSlots, u64 window, u32 width, u32 height)
      {
         GraphicsSystem* gfx = emplace<GraphicsSystem>(mem);
         if( !gfx )
         {
            CORE_LOG("Not enough memory for Graphics subsystem!");
            return nullptr;
         }

         // #temp until i convert the rest of the codebase
         gfx->m_staticMemory = mem;
         gfx->m_staticVertexMemory = allocateMemoryChunk(gfx->m_staticMemory, MegaBytes(10), 16);
         gfx->m_staticIndexMemory = allocateMemoryChunk(gfx->m_staticMemory, MegaBytes(10), 16);
         gfx->m_dynamicMemory = allocateMemoryChunk(gfx->m_staticMemory, MegaBytes(10), 16);

         gfx->m_dynamicVertexMemory = gfx->m_staticVertexMemory;
         gfx->m_dynamicIndexMemory = gfx->m_staticIndexMemory;

         gfx->m_window = window;
         gfx->m_backbufferSize.x = gfx->width = (f32)width;
         gfx->m_backbufferSize.y = gfx->height = (f32)height;
         gfx->m_backgroundColor.r = gfx->m_backgroundColor.g = gfx->m_backgroundColor.b = 0;
         clearCamera(gfx);

#define declare(identifier) \
   CORE_ASSERT_DBGERR(gfx->m_emptyInterfaceSlot < GraphicsSystem::InterfaceCount, "Reached maximum available interface declarations!"); \
   identifier = nullptr; \
   gfx->m_declaredObjects[gfx->m_emptyInterfaceSlot++] = (IUnknown**)&identifier

         declare(gfx->m_dxgiFactory);
         declare(gfx->m_dev);
         declare(gfx->m_devcon);
         declare(gfx->m_swapchain);
         declare(gfx->m_renderTarget);
         declare(gfx->m_samplerState);
         declare(gfx->m_depthStencilBuffer);
         declare(gfx->m_depthStencilView);
         declare(gfx->m_transparency);
         declare(gfx->m_cullingEnabled);
         declare(gfx->m_cullingDisabled);

#undef declare

         auto ok = initDevice(gfx);
         if( !ok )
         {
            return nullptr;
         }
         ok = initSwapChain(gfx);
         if( !ok )
         {
            return nullptr;
         }
         ok = initRenderTarget(gfx);
         if( !ok )
         {
            return nullptr;
         }
         ok = initViewport(gfx);
         if( !ok )
         {
            return nullptr;
         }
         ok = initSamplerState(gfx);
         if( !ok )
         {
            return nullptr;
         }

         gfx->m_renderer.init(gfx->m_dev, gfx->m_devcon, gfx->m_samplerState);

         gfx->m_textureFileLoader.init(gfx->m_dev);
         cache::init(&gfx->textures, gfx->m_staticMemory, textureSlots);

         gfx->m_vsLoader.init(gfx->m_dev);
         gfx->vertexShaders.init(gfx->m_staticMemory, shaderSlots);

         gfx->m_psLoader.init(gfx->m_dev);
         gfx->pixelShaders.init(gfx->m_staticMemory, shaderSlots);

         {
#include "graphics/shader/vertex/default_vertex_shader.h"

            auto layout = DefaultVertex::getDescription();
            CORE_ASSERT_DBGERR(layout.buffer != nullptr, "Error generating input layout information");
            auto defaultVertexShader = gfx->m_vsLoader.load(layout, (u8*)g_VShader, sizeof(g_VShader));

            gfx->m_defaultVertexShaderHandle = gfx->vertexShaders.insert(defaultVertexShader);
            CORE_ASSERT_DBGERR(gfx->m_defaultVertexShaderHandle.getIndex() == 0, "The default pixel shader should be at index 0");
            // #todo think about a sane way of using the default shader with different actual vertices being sent to the pipeline
         }

      {
#include "graphics/shader/pixel/default_pixel_shader.h"

         auto defaultPixelShader = gfx->m_psLoader.load((u8*)g_PShader, sizeof(g_PShader));
         gfx->m_defaultPixelShaderHandle = gfx->pixelShaders.insert(defaultPixelShader);
         CORE_ASSERT_DBGERR(gfx->m_defaultPixelShaderHandle.getIndex() == 0, "The default pixel shader should be at index 0");
      }


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

      HRESULT hr = gfx->m_dev->CreateBlendState(&blendDesc, &gfx->m_transparency);

      CORE_ASSERT_DBGERR(SUCCEEDED(hr), "Failed to create blend state for transparency!");

      D3D11_RASTERIZER_DESC rd;
      ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
      rd.FillMode = D3D11_FILL_SOLID;

      rd.CullMode = D3D11_CULL_BACK;
      hr = gfx->m_dev->CreateRasterizerState(&rd, &gfx->m_cullingEnabled);
      CORE_ASSERT_DBGERR(SUCCEEDED(hr), "Failed to create rasterizer state for cullingEnabled!");

      rd.CullMode = D3D11_CULL_NONE;
      hr = gfx->m_dev->CreateRasterizerState(&rd, &gfx->m_cullingDisabled);
      CORE_ASSERT_DBGERR(SUCCEEDED(hr), "Failed to create rasterizer state for cullingDisabled!");

      return gfx;
      }

      //*****************************************************************
      //          SHUTDOWN
      //*****************************************************************
      void shutdown(GraphicsSystem* gfx)
      {
         unload(gfx, gfx->m_defaultPixelShaderHandle);
         unload(gfx, gfx->m_defaultVertexShaderHandle);

         CORE_ASSERT_DBGWRN(gfx->pixelShaders.getCount() == 0, "Some pixel shaders were not released!");
         CORE_ASSERT_DBGWRN(gfx->vertexShaders.getCount() == 0, "Some vertex shaders were not released!");
         CORE_ASSERT_DBGWRN(cache::getUsedCount(&gfx->textures) == 0, "Some textures were not released!");

         gfx->m_renderer.shutdown();

         ID3D11Debug* debug = nullptr;
         HRESULT hr = gfx->m_dev->QueryInterface(IID_PPV_ARGS(&debug));
         while( gfx->m_emptyInterfaceSlot-- )
         {
            safeRelease(*gfx->m_declaredObjects[gfx->m_emptyInterfaceSlot]);
         }
         if( SUCCEEDED(hr) && debug != nullptr )
         {
            debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
            safeRelease(debug);
         }
         CORE_LOG_DEBUG("Graphics memory stats:");
         CORE_LOG_DEBUG("Static memory: ", gfx->m_staticMemory, "Dynamic memory: ", gfx->m_dynamicMemory);
      }

      void renderAllSubmitted(GraphicsSystem* gfx)
      {
         begin(gfx);

         //something

         present(gfx);
      }

      f32 getWindowWidth(GraphicsSystem* gfx)
      {
         return gfx->width;
      }
      
      f32 getWindowHeight(GraphicsSystem* gfx)
      {
         return gfx->height;
      }

      //*****************************************************************
      //          LOAD TEXTURE FROM FILE
      //*****************************************************************
      HTexture loadTextureFromFile(GraphicsSystem* gfx, str filename)
      {
         Texture t = gfx->m_textureFileLoader.load(filename);
         HTexture result = cache::insert(&gfx->textures, t);
         return result;
      }

      //*****************************************************************
      //          UNLOAD TEXTURE
      //*****************************************************************
      void unload(GraphicsSystem* gfx, HTexture handle)
      {
         if( !handle.isNull() )
         {
            Texture t = cache::remove(&gfx->textures, handle);
            gfx->m_textureFileLoader.unload(t);
         }
      }

      //*****************************************************************
      //          LOAD VERTEX SHADER FROM FILE
      //*****************************************************************
      HVertexShader loadVertexShaderFromFile(GraphicsSystem* gfx, str filename, VertexType vType)
      {
         auto fileMemory = gfx->m_dynamicMemory;
         auto file = loadFile(filename, fileMemory);
         CORE_ASSERT_ERR(file.memory != nullptr, "Not enough scratch memory to load shader file '", filename, "'");
         InputLayout layout{};
         switch( vType )
         {
            case VertexType::Default:
            {
               layout = DefaultVertex::getDescription();
            } break;
            case VertexType::Health:
            {
               layout = HealthVertex::getDescription();
            } break;
         }
         CORE_ASSERT_ERR(layout.buffer != nullptr && layout.size > 0, "Error generating input layout information");
         auto shader = gfx->m_vsLoader.load(layout, file.memory, file.size);

         auto result = gfx->vertexShaders.insert(shader);
         return result;
      }

      //*****************************************************************
      //          UNLOAD VERTEX SHADER
      //*****************************************************************
      void unload(GraphicsSystem* gfx, HVertexShader handle)
      {
         auto shader = gfx->vertexShaders.remove(handle);
         gfx->m_vsLoader.unload(shader);
      }

      //*****************************************************************
      //          LOAD PIXEL SHADER FROM FILE
      //*****************************************************************
      HPixelShader loadPixelShaderFromFile(GraphicsSystem* gfx, str filename)
      {
         auto fileMemory = gfx->m_dynamicMemory;
         auto file = loadFile(filename, fileMemory);
         CORE_ASSERT_ERR(file.memory != nullptr, "Not enough scratch memory to load shader file '", filename, "'");
         auto shader = gfx->m_psLoader.load(file.memory, file.size);

         auto result = gfx->pixelShaders.insert(shader);
         return result;
      }

      //*****************************************************************
      //          UNLOAD PIXEL SHADER
      //*****************************************************************
      void unload(GraphicsSystem* gfx, HPixelShader handle)
      {
         auto shader = gfx->pixelShaders.remove(handle);
         gfx->m_psLoader.unload(shader);
      }
      /************************************************************************
       *              ALLOCATE VERTEX BUFFER
       ************************************************************************/
      HealthVertexBuffer allocateVertexBuffer(GraphicsSystem* gfx, u32 size)
      {
         HealthVertexBuffer result;
         result.data = emplaceArray<HealthVertex>(gfx->m_dynamicVertexMemory, size);
         result.size = size;
         return result;
      }

      /************************************************************************
       *              ALLOCATE INDEX BUFFER
       ************************************************************************/
      IndexBuffer allocateIndexBuffer(GraphicsSystem* gfx, u32 size)
      {
         IndexBuffer result;
         result.data = emplaceArray<u32>(gfx->m_dynamicIndexMemory, size);
         result.size = size;
         return result;
      }

      /************************************************************************
       *              PROCESS FONT DESCRIPTOR
       ************************************************************************/
      HFont processFont(GraphicsSystem gfx, FontDescriptor font)
      {
         return{};
      }

      //*****************************************************************
      //          BEGIN
      //*****************************************************************
      void begin(GraphicsSystem* gfx)
      {
         gfx->m_devcon->ClearRenderTargetView(gfx->m_renderTarget, gfx->m_backgroundColor);
      }

      //*****************************************************************
      //          PRESENT
      //*****************************************************************
      void present(GraphicsSystem* gfx)
      {
         gfx->m_swapchain->Present(0, 0);
         gfx->m_dynamicIndexMemory = gfx->m_staticIndexMemory;
         gfx->m_dynamicVertexMemory = gfx->m_staticVertexMemory;
      }

      //*****************************************************************
      //          SET ORTHOGRAPHIC PROJECTION
      //*****************************************************************
      void setOrthographicProjection(GraphicsSystem* gfx)
      {
         gfx->m_renderer.setProjection(XMMatrixOrthographicLH(gfx->width, gfx->height, 1.0f, 100.0f));
      }

      //*****************************************************************
      //          SET PERSPECTIVE PROJECTION
      //*****************************************************************
      void setPerspectiveProjection(GraphicsSystem* gfx)
      {
         gfx->m_renderer.setProjection(XMMatrixPerspectiveFovLH(XMConvertToRadians(45), gfx->width / gfx->height, 1.0f, 100.0f));
      }

      //*****************************************************************
      //          APPLY CAMERA
      //*****************************************************************
      void applyCamera(GraphicsSystem* gfx, const Camera& cam)
      {
         gfx->m_renderer.setView(calculateCamView(cam));
      }

      //*****************************************************************
      //          CLEAR CAMERA
      //*****************************************************************
      void clearCamera(GraphicsSystem* gfx)
      {
         gfx->m_renderer.setView(XMMatrixIdentity());
      }

      //*****************************************************************
      //          WORLD TO SCREEN
      //*****************************************************************
      v2 worldToScreen(GraphicsSystem* gfx, const Camera& cam, v2 worldPos)
      {
         auto world = XMMatrixIdentity();
         //world *= XMMatrixScaling(transform.scale.x, transform.scale.y, 1.0f);
         //m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
         //m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
         //world *= XMMatrixRotationZ(transform.rotation);
         world *= XMMatrixTranslation(worldPos.x, worldPos.y, 0);
         world = world*gfx->m_renderer.getView()*gfx->m_renderer.getProjection();

         XMVECTOR position = XMVector3Project(convert(worldPos), 0, 0, gfx->width, gfx->height, 0.0f, 1.0f,
                                              gfx->m_renderer.getProjection(), calculateCamView(cam), XMMatrixIdentity());

         v2 result{position.m128_f32[0], position.m128_f32[1]};
         return result;
      }

      //*****************************************************************
      //          SCREEN TO WORLD
      //*****************************************************************
      v2 screenToWorld(GraphicsSystem* gfx, const Camera& cam, v2 screen)
      {
         auto objectSpace = XMVector3Unproject(convert(v3{screen.x, screen.y, 0.0f}), 0, 0, gfx->width, gfx->height, 0.0f, 1.0f,
                                               gfx->m_renderer.getProjection(), calculateCamView(cam), XMMatrixIdentity());
         auto camPos = convert(cam.getPosition());
         auto plane = XMPlaneFromPoints(convert({0, 0, 0}), convert({1, 0, 0}), convert({0, 1, 0}));
         auto loc = XMPlaneIntersectLine(plane, objectSpace, camPos);
         return v2{loc.m128_f32[0], loc.m128_f32[1]};
      }

      //*****************************************************************
      //          SET CULLING
      //*****************************************************************
      void setCulling(GraphicsSystem* gfx, bool isEnabled)
      {
         if( isEnabled )
         {
            gfx->m_renderer.setRasterizerState(gfx->m_cullingEnabled);
         }
         else
         {
            gfx->m_renderer.setRasterizerState(gfx->m_cullingDisabled);
         }
      }

      //*****************************************************************
      //          SET TRANSPARENCY
      //*****************************************************************
      void setTransparency(GraphicsSystem* gfx, bool isEnabled)
      {
         if( isEnabled )
         {
            gfx->m_renderer.setBlendState(gfx->m_transparency);
         }
         else
         {
            gfx->m_renderer.setBlendState(nullptr);
         }
      }

      //*****************************************************************
      //          RENDER MESH
      //*****************************************************************
      void renderMesh(GraphicsSystem* gfx, Transform t, Color c, const Mesh& mesh, Material material)
      {
         D3D_PRIMITIVE_TOPOLOGY d3dTopologies[] = {
            D3D_PRIMITIVE_TOPOLOGY_LINELIST,
            D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
            D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
            D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
         };

         auto vshader = gfx->vertexShaders.getData(material.vertexShaderHandle);
         auto pshader = gfx->pixelShaders.getData(material.pixelShaderHandle);
         auto texture = cache::get(&gfx->textures, material.textureHandle);
         auto topology = d3dTopologies[mesh.topology];

         gfx->m_renderer.setInputLayout(vshader._inputLayout);
         gfx->m_renderer.setShader(vshader._vertex);
         gfx->m_renderer.setShader(pshader._pixel);
         gfx->m_renderer.setTexture(texture._shaderResourceView);
         gfx->m_renderer.setVertexTopology(topology);
         gfx->m_renderer.render(t, c, mesh.vertices, mesh.indices);
      }
   }

   //*****************************************************************
   //          INIT DEVICE
   //*****************************************************************
   core_internal bool initDevice(GraphicsSystem* gfx)
   {
      HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&gfx->m_dxgiFactory);

      if( SUCCEEDED(hr) )
      {
         IDXGIAdapter* adapter = nullptr;
         hr = gfx->m_dxgiFactory->EnumAdapters(0, &adapter);
         if( SUCCEEDED(hr) )
         {
            auto driverType = D3D_DRIVER_TYPE_UNKNOWN;
            u32 flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
            flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
            hr = D3D11CreateDevice(adapter, driverType, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &gfx->m_dev, nullptr, &gfx->m_devcon);

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
   bool initSwapChain(GraphicsSystem* gfx)
   {
      DXGI_SWAP_CHAIN_DESC swapChainDesc;
      ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
      swapChainDesc.BufferCount = 1;
      swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
      swapChainDesc.OutputWindow = (HWND)gfx->m_window;
      swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
      swapChainDesc.Windowed = true;

      DXGI_MODE_DESC& bd = swapChainDesc.BufferDesc;
      bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      bd.Width = (u32)gfx->width; //m_backbufferSize.x
      bd.Height = (u32)gfx->height; //m_backbufferSize.y
      bd.RefreshRate.Denominator = 1;
      bd.RefreshRate.Numerator = 60;
      bd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
      bd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

      DXGI_SAMPLE_DESC& sd = swapChainDesc.SampleDesc;
      sd.Count = 1;
      sd.Quality = 0;

      safeRelease(gfx->m_swapchain);
      HRESULT hr = gfx->m_dxgiFactory->CreateSwapChain(gfx->m_dev, &swapChainDesc, &gfx->m_swapchain);
      if( FAILED(hr) )
      {
         CORE_LOG("initSwapChain failed");
      }
      return SUCCEEDED(hr);
   }

   //*****************************************************************
   //          INIT RENDER TARGET
   //*****************************************************************
   bool initRenderTarget(GraphicsSystem* gfx)
   {
      ID3D11Texture2D* bbTexture = nullptr;
      HRESULT hr = gfx->m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&bbTexture);
      if( SUCCEEDED(hr) )
      {
         safeRelease(gfx->m_renderTarget);
         hr = gfx->m_dev->CreateRenderTargetView(bbTexture, nullptr, &gfx->m_renderTarget);
         if( SUCCEEDED(hr) )
         {
            gfx->m_devcon->OMSetRenderTargets(1, &gfx->m_renderTarget, nullptr);
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
   bool initViewport(GraphicsSystem* gfx)
   {
      D3D11_VIEWPORT vp;
      ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
      vp.TopLeftX = 0;
      vp.TopLeftY = 0;
      vp.Width = gfx->width;
      vp.Height = gfx->height;
      vp.MinDepth = 0;
      vp.MaxDepth = 1;

      gfx->m_devcon->RSSetViewports(1, &vp);

      return true;
   }

   //*****************************************************************
   //          INIT SAMPLER STATE
   //*****************************************************************
   bool initSamplerState(GraphicsSystem* gfx)
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

      HRESULT hr = gfx->m_dev->CreateSamplerState(&sampd, &gfx->m_samplerState);

      if( FAILED(hr) )
      {
         CORE_LOG("initSamplerState failed");
      }
      return SUCCEEDED(hr);
   }

   //*****************************************************************
   //          INIT DEPTH BUFFER
   //*****************************************************************
   bool initDepthBuffer(GraphicsSystem* gfx)
   {
      D3D11_TEXTURE2D_DESC dsd;
      ZeroMemory(&dsd, sizeof(D3D11_TEXTURE2D_DESC));

      dsd.Width = (u32)gfx->width; //m_backbufferSize.x;
      dsd.Height = (u32)gfx->height; //m_backbufferSize.y;
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

      safeRelease(gfx->m_depthStencilBuffer);
      auto hr = gfx->m_dev->CreateTexture2D(&dsd, nullptr, &gfx->m_depthStencilBuffer);
      if( SUCCEEDED(hr) )
      {
         safeRelease(gfx->m_depthStencilView);
         hr = gfx->m_dev->CreateDepthStencilView(gfx->m_depthStencilBuffer, nullptr, &gfx->m_depthStencilView);
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






   namespace cache
   {
      void init(TextureCache* cache, Memory& m, u32 maxSlots)
      {
         cache->m_buffer = emplaceArray<Texture>(m, maxSlots);
         CORE_ASSERT_DBGERR(cache->m_buffer != nullptr, "Not enough memory for TextureCache storage!");
         cache->m_maxSlots = maxSlots;
         cache->m_usedSlots = 0;
      }

      HTexture insert(TextureCache* cache, Texture asset)
      {
         HTexture result{};

         if( cache->m_maxSlots != cache->m_usedSlots )
         {
            for( u16 i = 0; i < cache->m_maxSlots; ++i )
            {
               if( isUnloaded(cache->m_buffer[i]) )
               {
                  cache->m_buffer[i] = asset;
                  result.init(i);
                  ++cache->m_usedSlots;
                  break;
               }
            }
         }

         return result;
      }

      Texture remove(TextureCache* cache, HTexture handle)
      {
         Texture result = cache->m_buffer[handle.getIndex()];
         cache->m_buffer[handle.getIndex()] = {};
         --cache->m_usedSlots;
         return result;
      }

      Texture get(TextureCache* cache, HTexture handle)
      {
         Texture result = cache->m_buffer[handle.getIndex()];
         return result;
      }

      u32 getUsedCount(TextureCache* cache)
      {
         return cache->m_usedSlots;
      }

      HTexture find(TextureCache* cache, u32 assetId)
      {
         HTexture result{};
         for( u16 i = 0U; i < cache->m_maxSlots; ++i )
         {
            if( getAssetId(cache->m_buffer[i]) == assetId )
            {
               result.init(i);
               break;
            }
         }
         return result;
      }
   }
}

