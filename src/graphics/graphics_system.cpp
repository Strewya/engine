//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/graphics_system.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "graphics/mesh/mesh.h"
#include "graphics/shader/vertex/vertex_shader_loader.h"
#include "graphics/shader/pixel/pixel_shader_loader.h"
#include "graphics/camera.h"
#include "graphics/vertex.h"
#include "util/color.h"
#include "util/transform.h"
#include "util/utility.h"
#include "util/geometry/rect.h"
#include "window/window.h"
/******* end headers *******/

namespace core
{
   //*****************************************************************
   //          FILE STATIC FUNCTION DECLARATIONS
   //*****************************************************************
   static XMVECTOR convert(vec2f v);
   static XMVECTOR convert(Vec3f v);
   static XMMATRIX calculateCamView(const Camera& cam);

   //*****************************************************************
   //          INIT
   //*****************************************************************
   bool GraphicsSystem::init(WindowProxy window)
   {
      CORE_INIT_START(GraphicsSystem);

      m_window = window;
      m_backbufferSize.x = window.getSizeX();
      m_backbufferSize.y = window.getSizeY();
      m_backgroundColor.r = m_backgroundColor.g = m_backgroundColor.b = 0;
      clearCamera();

      //should be declared in reverse order due to how the cleanup is performed
      declare(m_cullingDisabled);
      declare(m_cullingEnabled);
      declare(m_transparency);
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

      CORE_STATUS_AND(_renderer.init(m_dev, m_devcon, m_samplerState));

      CORE_STATUS_AND(m_textureFileLoader.init(m_dev));
      CORE_STATUS_AND(m_vsLoader.init(m_dev));
      CORE_STATUS_AND(m_vsFileLoader.init(m_vsLoader));
      CORE_STATUS_AND(m_psLoader.init(m_dev));
      CORE_STATUS_AND(m_psFileLoader.init(m_psLoader));

      if( CORE_STATUS_OK )
      {
         auto defaultTexture = m_textureFileLoader.load(CORE_RESOURCE("Textures/defaultTexture.png"));
         CORE_STATUS_AND(textures.init(STR(TextureManager), m_textureFileLoader, defaultTexture));
      }


      VertexShader defaultVertexShader{nullptr, nullptr};
      if( CORE_STATUS_OK )
      {
#include "graphics/shader/vertex/default_vertex_shader.h"

         defaultVertexShader = m_vsLoader.load(DefaultVertex::getDescription(), (const char*)g_VShader, sizeof(g_VShader));
      }

      PixelShader defaultPixelShader{nullptr};
      if( CORE_STATUS_OK )
      {
#include "graphics/shader/pixel/default_pixel_shader.h"

         defaultPixelShader = m_psLoader.load((const char*)g_PShader, sizeof(g_PShader));
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


      CORE_STATUS_AND(vertexShaders.init(STR(VertexShaderManager), m_vsFileLoader, defaultVertexShader));
      CORE_STATUS_AND(pixelShaders.init(STR(PixelShaderManager), m_psFileLoader, defaultPixelShader));
      CORE_INIT_END(GraphicsSystem);
   }

   //*****************************************************************
   //          SHUTDOWN
   //*****************************************************************
   bool GraphicsSystem::shutdown()
   {
      CORE_SHUTDOWN_START(GraphicsSystem);

      CORE_STATUS_AND(pixelShaders.shutdown());
      CORE_STATUS_AND(vertexShaders.shutdown());
      CORE_STATUS_AND(textures.shutdown());

      CORE_STATUS_AND(m_psFileLoader.shutdown());
      CORE_STATUS_AND(m_psLoader.shutdown());
      CORE_STATUS_AND(m_vsFileLoader.shutdown());
      CORE_STATUS_AND(m_vsLoader.shutdown());
      CORE_STATUS_AND(m_textureFileLoader.shutdown());

      CORE_STATUS_AND(_renderer.shutdown());

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
      _renderer.setProjection(XMMatrixOrthographicLH((float)m_window.getSizeX(), (float)m_window.getSizeY(), 1.0f, 100.0f));
   }

   //*****************************************************************
   //          SET PERSPECTIVE PROJECTION
   //*****************************************************************
   void GraphicsSystem::setPerspectiveProjection()
   {
      _renderer.setProjection(XMMatrixPerspectiveFovLH(XMConvertToRadians(45), (float)m_window.getSizeX() / m_window.getSizeY(), 1.0f, 100.0f));
   }

   //*****************************************************************
   //          APPLY CAMERA
   //*****************************************************************
   void GraphicsSystem::applyCamera(const Camera& cam)
   {
      _renderer.setView(calculateCamView(cam));
   }

   //*****************************************************************
   //          CLEAR CAMERA
   //*****************************************************************
   void GraphicsSystem::clearCamera()
   {
      _renderer.setView(XMMatrixIdentity());
   }

   //*****************************************************************
   //          SET CULLING
   //*****************************************************************
   void GraphicsSystem::setCulling(bool isEnabled)
   {
      if( isEnabled )
      {
         _renderer.setRasterizerState(m_cullingEnabled);
      }
      else
      {
         _renderer.setRasterizerState(m_cullingDisabled);
      }
   }

   //*****************************************************************
   //          SET TRANSPARENCY
   //*****************************************************************
   void GraphicsSystem::setTransparency(bool isEnabled)
   {
      if( isEnabled )
      {
         _renderer.setBlendState(m_transparency);
      }
      else
      {
         _renderer.setBlendState(nullptr);
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

      _renderer.setInputLayout(vshader._inputLayout);
      _renderer.setShader(vshader._vertex);
      _renderer.setShader(pixelShaders.getData(mesh.pshader)._pixel);
      _renderer.setTexture(textures.getData(mesh.texture)._shaderResourceView);
      _renderer.setVertexTopology(topology[mesh.topology]);
      _renderer.render(t, c, mesh.vertices, mesh.indices);
   }

   void GraphicsSystem::renderText(const char* text, const FontDescriptor& fd, Rect box, TextJustification justify_x, TextJustification justify_y)
   {
      auto textLength = strlen(text);
      auto texture = textures.getData(fd.fontTexture);
      std::vector<HealthVertex> vertices;
      vertices.reserve(textLength * 4);
      
      float textureWidth = (float)texture.width;
      float textureHeight = (float)texture.height;

      auto generateVertices = [&](uint32_t start, uint32_t end, float x, float y) -> float
      {
         while( start != end )
         {
            char character = text[start];
            Rect characterRect = fd.glyphs[character - 32];
            float tv_top = characterRect.bottom() / textureHeight;
            float tv_bot = characterRect.top() / textureHeight;
            float tu_left = characterRect.left() / textureWidth;
            float tu_rght = characterRect.right() / textureWidth;
            float characterWidth = characterRect.halfSize.x * 2;
            float characterHeight = characterRect.halfSize.y * 2;

            vertices.push_back({{x, y - characterHeight, 0}, {1, 1, 1, 1}, {tu_left, tv_bot}, 0});
            vertices.push_back({{x, y - 0, 0}, {1, 1, 1, 1}, {tu_left, tv_top}, 0});
            x += characterWidth;
            vertices.push_back({{x, y - 0, 0}, {1, 1, 1, 1}, {tu_rght, tv_top}, 0});
            vertices.push_back({{x, y - characterHeight, 0}, {1, 1, 1, 1}, {tu_rght, tv_bot}, 0});

            ++start;
         }
         return x;
      };

      auto justifyVerts = [&](uint32_t vert, float lineEnd, float boxHW)
      {
         lineEnd += boxHW;
         auto boxEnd = boxHW * 2;
         auto offset = (boxEnd - lineEnd)*0.5f*justify_x;
         for( ; vert < vertices.size(); ++vert )
         {
            vertices[vert].position.x += offset;
         }
      };



      uint32_t characterIndex = 0;
      uint32_t lineStart = 0;
      float boxWidth = box.halfSize.x * 2;
      float boxHeight = box.halfSize.y * 2;
      float currentLinePosX = -box.halfSize.x;
      float currentLinePosY = box.halfSize.y;

      float currentLineWidth = 0;
      uint32_t lastValidBreakpoint = 0;
      for( ;; )
      {
         char character = text[characterIndex];
         if( character == 0 )
         {
            auto vertStart = vertices.size();
            auto x = generateVertices(lineStart, characterIndex, currentLinePosX, currentLinePosY);
            justifyVerts(vertStart, x, box.halfSize.x);
            break;
         }
         Rect characterRect = fd.glyphs[character - 32];
         float characterWidth = characterRect.halfSize.x * 2;
         if( character == ' ' )
         {
            lastValidBreakpoint = characterIndex;
         }
         if( currentLineWidth + characterWidth >= boxWidth )
         {
            auto vertStart = vertices.size();
            auto x = generateVertices(lineStart, lastValidBreakpoint, currentLinePosX, currentLinePosY);
            justifyVerts(vertStart, x, box.halfSize.x);
            characterIndex = lineStart = lastValidBreakpoint + 1;
            currentLineWidth = 0;
            currentLinePosX = -box.halfSize.x;
            currentLinePosY -= fd.height;
         }
         else
         {
            currentLineWidth += characterWidth;
            ++characterIndex;
         }
      }



      
      /*float offset = (bw - cw)*0.5f*justify_x;
      for( uint32_t mv = lineStart; mv < v; ++mv )
      {
      vertices[mv].position.x += offset;
      }*/

      std::vector<uint32_t> indices;
      indices.reserve(textLength * 6);
      for( uint32_t i = 0; i < textLength; ++i )
      {
         auto x = i * 4;
         indices.emplace_back(x + 0);
         indices.emplace_back(x + 1);
         indices.emplace_back(x + 2);
         indices.emplace_back(x + 2);
         indices.emplace_back(x + 3);
         indices.emplace_back(x + 0);
      }

      auto vshader = vertexShaders.getData(fd.vshader);
      auto pshader = pixelShaders.getData(fd.pshader);

      _renderer.setInputLayout(vshader._inputLayout);
      _renderer.setShader(vshader._vertex);
      _renderer.setShader(pshader._pixel);
      _renderer.setTexture(texture._shaderResourceView);
      _renderer.setVertexTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      _renderer.render(Transform{vec2f{0, 0}, {1, 1}}, {}, vertices, indices);
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
      swapChainDesc.OutputWindow = m_window.getRawWindow()->getWindowHandle();
      swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
      swapChainDesc.Windowed = true;

      DXGI_MODE_DESC& bd = swapChainDesc.BufferDesc;
      bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      bd.Width = m_window.getSizeX(); //m_backbufferSize.x
      bd.Height = m_window.getSizeY(); //m_backbufferSize.y
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
      vp.Width = (float)m_window.getSizeX();
      vp.Height = (float)m_window.getSizeY();
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

      dsd.Width = m_window.getSizeX(); //m_backbufferSize.x;
      dsd.Height = m_window.getSizeY(); //m_backbufferSize.y;
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
   XMVECTOR convert(vec2f v)
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

#include "lua/lua_system.h"

namespace core
{
   FontDescriptor loadFont(const char* filename, HTexture texture)
   {
      FontDescriptor result{};

      LuaSystem LS;
      LS.init();

      auto lua = LS.getStack();
      if( !lua.doFile(filename) )
      {
         auto str = lua.to<std::string>();
         lua.pop();
         CORE_INFO(str);
         return result;
      }

      for( lua.pairs(); lua.next(); lua.pop(1) )
      {
         if( !lua.is<std::string>(-2) || !lua.is<LuaTable>(-1) )
         {
            return result;
         }
         result.height = get(lua, "size", 0);
         if( result.height == 0 )
         {
            return result;
         }
         for( lua.ipairs("glyphs"); lua.next(); lua.pop(1) )
         {
            if( lua.is<uint32_t>(-2) && lua.is<LuaTable>(-1) )
            {
               auto ascii = get<char>(lua, "char", 0);
               auto left = get(lua, "left", -1);
               auto right = get(lua, "right", -1);
               auto top = get(lua, "top", -1);
               if( ascii != 0 && left != -1 && right != -1 && top != -1 )
               {
                  auto i = ascii - 32;
                  result.glyphs[i].center.set((left + right)*0.5f, top + result.height*0.5f);
                  result.glyphs[i].halfSize.set((right - left)*0.5f, result.height*0.5f);
               }
            }
         }
      }
      lua.pop();
      result.fontTexture = texture;
      LS.shutdown();

      return result;
   }
}
