//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Modules/Rendering/Service/D3D11/DXRenderer.h"
	/*** C++ headers ***/
#include <algorithm>
#include <stdexcept>
	/*** extra headers ***/
#include "Modules/Rendering/Service/D3D11/Vertex.h"
	/*** end headers ***/


namespace Graphics
{
#define TEST_SUCCESS(result) if(FAILED(result)) return false
#define SAFE_RELEASE(ptr) if(ptr != nullptr) { ptr->Release(); ptr = nullptr; }

	DXRenderer::DXRenderer(HWND hwnd, uint32_t screenW, uint32_t screenH)
		: m_screenW(screenW), m_screenH(screenH),
		m_hwnd(hwnd), m_dev(nullptr), m_devcon(nullptr), m_swapchain(nullptr), m_renderTarget(nullptr),
		m_vertexShaderBlob(nullptr), m_vertexShader(nullptr), m_pixelShaderBlob(nullptr), m_pixelShader(nullptr),
		m_cwCulling(nullptr), m_ccwCulling(nullptr), m_noCulling(nullptr),
		m_inputLayout(nullptr), m_vertexBuffer(nullptr), m_indexBuffer(nullptr),
		m_depthStencilView(nullptr), m_depthStencilBuffer(nullptr),
		m_cbPerObjectBuffer(nullptr),
		m_shaderResourceView(nullptr), m_samplerState(nullptr),
		m_backgroundColor(0,0,0,1),
		m_camProjection(XMMatrixIdentity()), m_camView(XMMatrixIdentity())
	{
		if(!init())
		{
			throw std::logic_error("Error creating D3D device and swap chain");
		}

		if(!initPipeline())
		{
			throw std::logic_error("Error initializing pipeline");
		}
	}

	DXRenderer::~DXRenderer()
	{
		close();
	}

	//*****************************************************************
	//					INITIALIZATION
	//*****************************************************************
	bool DXRenderer::init()
	{
		fillSwapChainDesc(m_swapChainDesc);

		auto driverType = D3D_DRIVER_TYPE_HARDWARE;
		uint32_t flags = 0;

#ifdef _DEBUG
		flags = D3D11_CREATE_DEVICE_DEBUG;
#endif
		HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &m_swapChainDesc, &m_swapchain, &m_dev, nullptr, &m_devcon);
		TEST_SUCCESS(hr);
		
		ID3D11Texture2D* bbTexture = nullptr;
		hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&bbTexture);
		TEST_SUCCESS(hr);
		
		hr = m_dev->CreateRenderTargetView(bbTexture, nullptr, &m_renderTarget);
		TEST_SUCCESS(hr);

		hr = bbTexture->Release();
		

		D3D11_VIEWPORT vp;
		ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = (float)m_screenW;
		vp.Height = (float)m_screenH;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;

		m_devcon->RSSetViewports(1, &vp);

		return true;
	}

	void DXRenderer::fillSwapChainDesc(DXGI_SWAP_CHAIN_DESC& scd)
	{
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));		
		scd.BufferCount		= 1;
		scd.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.Flags			= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.OutputWindow	= m_hwnd;
		scd.SwapEffect		= DXGI_SWAP_EFFECT_DISCARD;
		scd.Windowed		= true;

		DXGI_MODE_DESC& bd = scd.BufferDesc;
		bd.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
		bd.Width					= m_screenW;
		bd.Height					= m_screenH;
		bd.RefreshRate.Denominator	= 1;
		bd.RefreshRate.Numerator	= 60;
		bd.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bd.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC& sd = scd.SampleDesc;
		sd.Count	= 1;
		sd.Quality	= 0;
	}

	bool DXRenderer::initPipeline()
	{
		//********************************** VERTEX SHADER
		HRESULT hr = 0;
		std::string shaderPath = "../resources/Shaders/shader.hlsl";
		hr = D3DX11CompileFromFile(shaderPath.c_str(), nullptr, nullptr, "VShader", "vs_4_0", 0, 0, nullptr, &m_vertexShaderBlob, nullptr, nullptr);
		TEST_SUCCESS(hr);

		hr = m_dev->CreateVertexShader(m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize(), nullptr, &m_vertexShader);
		TEST_SUCCESS(hr);



		//********************************** PIXEL SHADER
		hr = D3DX11CompileFromFile(shaderPath.c_str(), nullptr, nullptr, "PShader", "ps_4_0", 0, 0, nullptr, &m_pixelShaderBlob, nullptr, nullptr);
		TEST_SUCCESS(hr);

		hr = m_dev->CreatePixelShader(m_pixelShaderBlob->GetBufferPointer(), m_pixelShaderBlob->GetBufferSize(), nullptr, &m_pixelShader);
		TEST_SUCCESS(hr);
		
		m_devcon->VSSetShader(m_vertexShader, nullptr, 0);
		m_devcon->PSSetShader(m_pixelShader, nullptr, 0);

		//********************************** INPUT LAYOUT
		std::vector<D3D11_INPUT_ELEMENT_DESC> ied = Vertex::getDescription();

		hr = m_dev->CreateInputLayout(&ied[0], ied.size(), m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize(), &m_inputLayout);
		TEST_SUCCESS(hr);
		m_devcon->IASetInputLayout(m_inputLayout);


		//********************************** VERTEX BUFFER
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth		= sizeof(Vertex) * 30;
		bd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags		= 0;
		bd.Usage			= D3D11_USAGE_DYNAMIC;
		
		hr = m_dev->CreateBuffer(&bd, nullptr, &m_vertexBuffer);
		TEST_SUCCESS(hr);

		//********************************** INDEX BUFFER
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
		bd.ByteWidth		= sizeof(uint32_t) * 60;
		bd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags		= 0;
		bd.Usage			= D3D11_USAGE_DYNAMIC;

		hr = m_dev->CreateBuffer(&bd, nullptr, &m_indexBuffer);
		TEST_SUCCESS(hr);

		//********************************** RASTERIZER STATE
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		
		rd.FrontCounterClockwise = true;
		hr = m_dev->CreateRasterizerState(&rd, &m_ccwCulling);
		TEST_SUCCESS(hr);

		rd.FrontCounterClockwise = false;
		hr = m_dev->CreateRasterizerState(&rd, &m_cwCulling);
		TEST_SUCCESS(hr);
		
		rd.CullMode = D3D11_CULL_NONE;
		hr = m_dev->CreateRasterizerState(&rd, &m_noCulling);
		TEST_SUCCESS(hr);

		m_devcon->RSSetState(m_ccwCulling);
		
		//********************************** BLENDING
		D3D11_BLEND_DESC bld;
		ZeroMemory(&bld, sizeof(D3D11_BLEND_DESC));

		bld.AlphaToCoverageEnable = false;

		D3D11_RENDER_TARGET_BLEND_DESC& rtbd = bld.RenderTarget[0];
		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
		rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
		rtbd.BlendOp = D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

		hr = m_dev->CreateBlendState(&bld, &m_blendState);
		TEST_SUCCESS(hr);

		//********************************** DEPTH STENCIL
		D3D11_TEXTURE2D_DESC dsd;
		ZeroMemory(&dsd, sizeof(D3D11_TEXTURE2D_DESC));

		dsd.Width			= m_screenW;
		dsd.Height			= m_screenH;
		dsd.MipLevels		= 1;
		dsd.ArraySize		= 1;
		dsd.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsd.Usage			= D3D11_USAGE_DEFAULT;
		dsd.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
		dsd.CPUAccessFlags	= 0;
		dsd.MiscFlags		= 0;

		DXGI_SAMPLE_DESC& sd = dsd.SampleDesc;
		sd.Count	= 1;
		sd.Quality	= 0;

		hr = m_dev->CreateTexture2D(&dsd, nullptr, &m_depthStencilBuffer);
		TEST_SUCCESS(hr);

		hr = m_dev->CreateDepthStencilView(m_depthStencilBuffer, nullptr, &m_depthStencilView);

		m_devcon->OMSetRenderTargets(1, &m_renderTarget, nullptr);

		
		//********************************** CONSTANT BUFFER FOR MATRICES
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage			= D3D11_USAGE_DEFAULT;
		bd.ByteWidth		= sizeof(cbPerObject);
		bd.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags	= 0;
		bd.MiscFlags		= 0;
		
		hr = m_dev->CreateBuffer(&bd, nullptr, &m_cbPerObjectBuffer);
		TEST_SUCCESS(hr);

		m_camPosition = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		m_camLookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		m_camView = XMMatrixLookAtLH(m_camPosition, m_camLookAt, m_camUp);
		//_camProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45), (float)_screenW/_screenH, 1.0f, 100.0f);
		m_camProjection = XMMatrixOrthographicLH((float)m_screenW, (float)m_screenH, 1.0f, 100.0f);
		



		//********************************** CONSTANT BUFFER FOR MATRICES
		hr = D3DX11CreateShaderResourceViewFromFile(m_dev, "../resources/character_t.png", nullptr, nullptr, &m_shaderResourceView, nullptr);
		TEST_SUCCESS(hr);

		D3D11_SAMPLER_DESC sampd;
		ZeroMemory(&sampd, sizeof(D3D11_SAMPLER_DESC));
		sampd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampd.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampd.MinLOD = 0;
		sampd.MaxLOD = D3D11_FLOAT32_MAX;

		hr = m_dev->CreateSamplerState(&sampd, &m_samplerState);
		TEST_SUCCESS(hr);

		return true;
	}

	//*****************************************************************
	//					RESETTING
	//*****************************************************************
	bool DXRenderer::reset()
	{
		
		return true;
	}

	//*****************************************************************
	//					SHUTTING DOWN
	//*****************************************************************

	void DXRenderer::close()
	{
		if(m_swapchain != nullptr)
		{
			m_swapchain->SetFullscreenState(false, nullptr);
		}

		SAFE_RELEASE(m_samplerState);
		SAFE_RELEASE(m_shaderResourceView);
		SAFE_RELEASE(m_cbPerObjectBuffer);
		SAFE_RELEASE(m_depthStencilBuffer);
		SAFE_RELEASE(m_depthStencilView);
		SAFE_RELEASE(m_indexBuffer);
		SAFE_RELEASE(m_vertexBuffer);
		SAFE_RELEASE(m_inputLayout);
		SAFE_RELEASE(m_noCulling);
		SAFE_RELEASE(m_cwCulling);
		SAFE_RELEASE(m_ccwCulling);
		SAFE_RELEASE(m_pixelShader);
		SAFE_RELEASE(m_pixelShaderBlob);
		SAFE_RELEASE(m_vertexShader);
		SAFE_RELEASE(m_vertexShaderBlob);
		SAFE_RELEASE(m_renderTarget);
		SAFE_RELEASE(m_swapchain);
		SAFE_RELEASE(m_devcon);
		SAFE_RELEASE(m_dev);
		m_hwnd = 0;
	}

	//*****************************************************************
	//					BEGIN SCENE
	//*****************************************************************
	bool DXRenderer::BeginScene()
	{
		m_devcon->ClearRenderTargetView(m_renderTarget, m_backgroundColor);
		//_devcon->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1, 0);
		return true;
	}
	
	//*****************************************************************
	//					END SCENE
	//*****************************************************************
	void DXRenderer::EndScene()
	{
		m_swapchain->Present(0, 0);
	}
	
	
	
	
	//*****************************************************************
	//					TEST DRAW
	//*****************************************************************
	void DXRenderer::testDraw()
	{
		Vertex vertices[] =
		{
			Vertex( 0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f ),
			Vertex( 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f ),
			Vertex( -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f ),
		};

		D3D11_MAPPED_SUBRESOURCE ms;
		m_devcon->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		memcpy(ms.pData, vertices, sizeof(vertices));
		m_devcon->Unmap(m_vertexBuffer, 0);

		uint32_t stride = sizeof(Vertex);
		uint32_t offset = 0;
		m_devcon->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		m_devcon->PSSetShaderResources(0, 1, &m_shaderResourceView);
		
		m_devcon->Draw(3, 0);
	}

	




	//*****************************************************************
	//					GET SCREEN SIZE
	//*****************************************************************
	Util::Vec2 DXRenderer::getScreenSize() const
	{
		return Util::Vec2((float)m_screenW, (float)m_screenH);
	}

	//*****************************************************************
	//					SET  SCREEN SIZE
	//*****************************************************************
	void DXRenderer::setScreenSize(const Util::Vec2& size)
	{
		setScreenSize((uint32_t)size.x, (uint32_t)size.y);
	}

	void DXRenderer::setScreenSize(uint32_t width, uint32_t height)
	{
		
		
	}

	//*****************************************************************
	//					GET BACKGROUND COLOR
	//*****************************************************************
	Util::Color DXRenderer::getBackgroundColor() const
	{
		return Util::Color((const float*)m_backgroundColor);
	}

	//*****************************************************************
	//					SET BACKGROUND COLOR
	//*****************************************************************
	void DXRenderer::setBackgroundColor(const Util::Color& color)
	{
		m_backgroundColor = color.getRGBA();
		m_backgroundColor.a = 1.0f;
	}

	void DXRenderer::setBackgroundColor(float red, float green, float blue)
	{
		m_backgroundColor = D3DXCOLOR(red, green, blue, 1.0f);
	}
	
	//*****************************************************************
	//					GET FULLSCREEN STATE
	//*****************************************************************
	bool DXRenderer::getFullscreenState() const 
	{
		BOOL fs = false;
		m_swapchain->GetFullscreenState(&fs, nullptr);
		return fs == TRUE;
	}

	//*****************************************************************
	//					SET FULLSCREEN STATE
	//*****************************************************************
	void DXRenderer::setFullscreenState(bool state)
	{
		if(getFullscreenState() != state)
		{
			m_swapchain->SetFullscreenState(state, nullptr);
		}
	}

	//*****************************************************************
	//					CREATE VERTEX BUFFER
	//*****************************************************************
	uint32_t DXRenderer::createVertexBuffer(uint32_t size)
	{
		for(auto it = m_vertexBuffers.begin(); it != m_vertexBuffers.end(); ++it)
		{
			if(*it == nullptr)
			{
				//make vertex buffer at this location, then return from the method
				it->reset(new VertexBuffer());
				(*it)->id = (it - m_vertexBuffers.begin());
				(*it)->buffer = makeVertexBuffer(size);
				(*it)->devcon = m_devcon;
				return (*it)->id;
			}
		}
		//no slot found
		m_vertexBuffers.push_back(std::unique_ptr<VertexBuffer>(new VertexBuffer()));
		m_vertexBuffers.back()->id = m_vertexBuffers.size()-1;
		m_vertexBuffers.back()->buffer = makeVertexBuffer(size);
		m_vertexBuffers.back()->devcon = m_devcon;
		return m_vertexBuffers.back()->id;
	}

	//*****************************************************************
	//					GET VERTEX BUFFER
	//*****************************************************************
	IVertexBuffer& DXRenderer::getVertexBuffer(uint32_t id)
	{
		assert(id < m_vertexBuffers.size() && m_vertexBuffers[id] != nullptr);
		return *m_vertexBuffers[id];
	}

	//*****************************************************************
	//					DESTROY VERTEX BUFFER
	//*****************************************************************
	void DXRenderer::destroyVertexBuffer(uint32_t id)
	{
		assert(id < m_vertexBuffers.size());
		if(m_vertexBuffers[id] != nullptr)
		{
			m_vertexBuffers[id].reset(nullptr);
		}
	}

	//*****************************************************************
	//					MAKE VERTEX BUFFER INTERNAL
	//*****************************************************************
	ID3D11Buffer* DXRenderer::makeVertexBuffer(uint32_t size)
	{
		assert(size > 0);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth		= sizeof(Vertex) * size;
		desc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags		= 0;
		desc.Usage			= D3D11_USAGE_DYNAMIC;
		
		ID3D11Buffer* buffer = nullptr;
		HRESULT hr = m_dev->CreateBuffer(&desc, nullptr, &buffer);
		assert(SUCCEEDED(hr));
		return buffer;
	}
	
	//*****************************************************************
	//					CREATE INDEX BUFFER
	//*****************************************************************
	uint32_t DXRenderer::createIndexBuffer(uint32_t size)
	{
		for(auto it = m_indexBuffers.begin(); it != m_indexBuffers.end(); ++it)
		{
			if(*it == nullptr)
			{
				//make vertex buffer at this location, then return from the method
				it->reset(new IndexBuffer());
				(*it)->id = (it - m_indexBuffers.begin());
				(*it)->buffer = makeIndexBuffer(size);
				(*it)->devcon = m_devcon;
				return (*it)->id;
			}
		}
		//no slot found
		m_indexBuffers.push_back(std::unique_ptr<IndexBuffer>(new IndexBuffer()));
		m_indexBuffers.back()->id = m_indexBuffers.size()-1;
		m_indexBuffers.back()->buffer = makeIndexBuffer(size);
		m_indexBuffers.back()->devcon = m_devcon;
		return m_indexBuffers.back()->id;
	}

	//*****************************************************************
	//					GET INDEX BUFFER
	//*****************************************************************
	IIndexBuffer& DXRenderer::getIndexBuffer(uint32_t id)
	{
		assert(id < m_indexBuffers.size() && m_indexBuffers[id] != nullptr);
		return *m_indexBuffers[id];
	}

	//*****************************************************************
	//					DESTROY INDEX BUFFER
	//*****************************************************************
	void DXRenderer::destroyIndexBuffer(uint32_t id)
	{
		assert(id < m_indexBuffers.size());
		if(m_indexBuffers[id] != nullptr)
		{
			m_indexBuffers[id].reset(nullptr);
		}
	}

	//*****************************************************************
	//					MAKE INDEX BUFFER INTERNAL
	//*****************************************************************
	ID3D11Buffer* DXRenderer::makeIndexBuffer(uint32_t size)
	{
		assert(size > 0);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags		= D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth		= sizeof(uint32_t) * size;
		desc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags		= 0;
		desc.Usage			= D3D11_USAGE_DYNAMIC;
		
		ID3D11Buffer* buffer = nullptr;
		HRESULT hr = m_dev->CreateBuffer(&desc, nullptr, &buffer);
		assert(SUCCEEDED(hr));
		return buffer;
	}

	void DXRenderer::setIdentity()
	{
		m_world = XMMatrixIdentity();
	}

	void DXRenderer::setTranslation(const Util::Vec3& translation)
	{
		setTranslation(translation.x, translation.y, translation.z);
	}

	void DXRenderer::setScaling(const Util::Vec3& scale)
	{
		setScaling(scale.x, scale.y, scale.z);
	}

	void DXRenderer::setRotation(const Util::Vec3& rot)
	{
		setRotation(rot.x, rot.y, rot.z);
	}

	void DXRenderer::setTranslation(float translationX,float translationY, float translationZ)
	{
		m_world *= XMMatrixTranslation(translationZ, translationZ, translationZ);
	}

	void DXRenderer::setScaling(float scaleX, float scaleY, float scaleZ)
	{
		m_world *= XMMatrixScaling(scaleX, scaleY, scaleZ);
	}

	void DXRenderer::setRotation(float rotationX, float rotationY, float rotationZ)
	{
		m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		m_world *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
	}

	void DXRenderer::applyTransform()
	{
		cbPerObject cb;
		cb.WVP = XMMatrixTranspose(m_world * m_camView * m_camProjection);

		m_devcon->UpdateSubresource(m_cbPerObjectBuffer, 0, nullptr, &cb, 0, 0);
		m_devcon->VSSetConstantBuffers(0, 1, &m_cbPerObjectBuffer);
	}



	/*
	DXRenderer::DXRenderer(HWND hwnd)
		: _hwnd(hwnd), _d3d(nullptr), _d3ddev(nullptr), _backbuffer(nullptr), _spriteHandler(nullptr), _line(nullptr),
		_vertexFormat(0), _transparentColor(D3DCOLOR_XRGB(255,0,255)), _backgroundFillColor(D3DCOLOR_XRGB(0,0,0))
	{
		if(!InitDevices())
		{
			throw std::runtime_error("DirectX graphics subsystem has failed to load.");
		}

		_textures.setD3DDevice(_d3ddev);
	}

	DXRenderer::~DXRenderer()
	{
		_textures.clear();

		if(_line != nullptr)
		{
			_line->Release();
			_line = nullptr;
		}

		if(_spriteHandler != nullptr)
		{
			_spriteHandler->Release();
			_spriteHandler = nullptr;
		}

		if(_backbuffer != nullptr)
		{
			_backbuffer->Release();
			_backbuffer = nullptr;
		}

		if(_d3ddev != nullptr)
		{
			_d3ddev->Release();
			_d3ddev = nullptr;
		}
		
		if(_d3d != nullptr)
		{
			_d3d->Release();
			_d3d = nullptr;
		}
	}

	//*****************************************************************
	//		INIT DEVICES
	//*****************************************************************
	bool DXRenderer::InitDevices()
	{
		_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(_d3d==nullptr)
		{
			return false;
		}
		//set Direct3D presentation parameters
		ZeroMemory(&_d3dpp, sizeof(_d3dpp));
		_d3dpp.Windowed					= true;//(!_settings.IsFullscreen());
		_d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		_d3dpp.BackBufferFormat			= D3DFMT_R5G6B5; //D3DFMT_X8R8G8B8;
		_d3dpp.BackBufferCount			= 1;
		_d3dpp.BackBufferWidth			= 1024;//_settings.GetWidth();
		_d3dpp.BackBufferHeight			= 768;//_settings.GetHeight();
		_d3dpp.hDeviceWindow			= _hwnd;
		_d3dpp.EnableAutoDepthStencil	= true;
		_d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
		_d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT; //_IMMEDIATE
		_d3dpp.MultiSampleQuality		= 0;
		_d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
		_d3dpp.Flags					= 0;
		//create Direct3D device
		if(FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &_d3dpp, &_d3ddev)))
		{
			return false;
		}
		if(_d3ddev == nullptr)
			return false;
		//enable the depth buffer
		if(FAILED(_d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE)))
			return false;
		//clear the backbuffer to black
		if(FAILED(_d3ddev->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0)))
			return false;
		//create pointer to backbuffer
		if(FAILED(_d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &_backbuffer)))
			return false;
		//create pointer to sprite class
		if(FAILED(D3DXCreateSprite(_d3ddev, &_spriteHandler)))
			return false;
		//create pointer to line class
		if(FAILED(D3DXCreateLine(_d3ddev, &_line)))
			return false;
		//create a default texture to use when another texture fails to load
		//should be embedded into the DLL if possible
		if(FAILED(D3DXCreateTexture(_d3ddev, 1, 1, 0, D3DPOOL_DEFAULT, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, &_defaultTexture)))
			return false;

		D3DXMatrixIdentity(&_transformMatrix);
		return true;
	}

	//*****************************************************************
	//					RESET DEVICES
	//*****************************************************************
	bool DXRenderer::ResetDevices()
	{
		_spriteHandler->OnLostDevice();
		_line->OnLostDevice();
		_backbuffer->Release();
		if(FAILED(_d3ddev->Reset(&_d3dpp)))
			return false;
		if(FAILED(_d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &_backbuffer)))
			return false;
		_spriteHandler->OnResetDevice();
		_line->OnResetDevice();
		uint32_t style, exStyle;
		if(!_d3dpp.Windowed)
		{
			style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
			exStyle = WS_EX_APPWINDOW;
		}
		else
		{
			style = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
			exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		}
		RECT rc = {0,0, (LONG)_d3dpp.BackBufferWidth, (LONG)_d3dpp.BackBufferHeight };
		AdjustWindowRectEx(&rc, style, false, exStyle);
		SetWindowLong(_d3dpp.hDeviceWindow, GWL_STYLE, style);
		SetWindowLong(_d3dpp.hDeviceWindow, GWL_EXSTYLE, exStyle);
		SetWindowPos(_d3dpp.hDeviceWindow, HWND_NOTOPMOST, 0, 0, rc.right-rc.left, rc.bottom-rc.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		return true;
	}

	//*****************************************************************
	//					GET TEXTURE CACHE
	//*****************************************************************
	ITextureCache& DXRenderer::getTextureCache()
	{
		return _textures;
	}

	//*****************************************************************
	//					GET SCREEN SIZE
	//*****************************************************************
	Util::Vec2 DXRenderer::getScreenSize() const
	{
		return Util::Vec2((float)_d3dpp.BackBufferWidth, (float)_d3dpp.BackBufferHeight);
	}

	//*****************************************************************
	//					SET  SCREEN SIZE
	//*****************************************************************
	void DXRenderer::setScreenSize(const Util::Vec2& size)
	{
		setScreenSize((uint32_t)size.x, (uint32_t)size.y);
	}

	void DXRenderer::setScreenSize(uint32_t width, uint32_t height)
	{
		_d3dpp.BackBufferWidth = width;
		_d3dpp.BackBufferHeight = height;
		ResetDevices();
	}

	//*****************************************************************
	//					GET TRANSPARENT COLOR
	//*****************************************************************
	Util::Color DXRenderer::getTransparentColor() const
	{
		return Util::Color(_transparentColor);
	}

	//*****************************************************************
	//					SET TRANSPARENT COLOR
	//*****************************************************************
	void DXRenderer::setTransparentColor(const Util::Color& color)
	{
		Util::Color c = color;
		c.setAlpha(255);
		_transparentColor = c.getARGB();
	}

	void DXRenderer::setTransparentColor(uint8_t red, uint8_t green, uint8_t blue)
	{
		setTransparentColor(Util::Color(red, green, blue));
	}

	//*****************************************************************
	//					GET BACKGROUND COLOR
	//*****************************************************************
	Util::Color DXRenderer::getBackgroundFillColor() const
	{
		return Util::Color(_backgroundFillColor);
	}

	//*****************************************************************
	//					SET BACKGROUND COLOR
	//*****************************************************************
	void DXRenderer::setBackgroundFillColor(const Util::Color& color)
	{
		Util::Color c = color;
		c.setAlpha(255);
		_backgroundFillColor = c.getARGB();
	}

	void DXRenderer::setBackgroundFillColor(uint8_t red, uint8_t green, uint8_t blue)
	{
		setBackgroundFillColor(Util::Color(red, green, blue));
	}

	//*****************************************************************
	//					GET FULLSCREEN STATE
	//*****************************************************************
	bool DXRenderer::getFullscreenState() const 
	{
		return !_d3dpp.Windowed;
	}

	//*****************************************************************
	//					SET FULLSCREEN STATE
	//*****************************************************************
	void DXRenderer::setFullscreenState(bool state)
	{
		if(_d3dpp.Windowed != (BOOL)state)
		{
			_d3dpp.Windowed = state;
			ResetDevices();
		}
	}

	//*****************************************************************
	//					MAKE FONT
	//*****************************************************************
	uint32_t DXRenderer::MakeFont(const char* name, uint32_t size, uint32_t weight, bool italic)
	{
		int index = _fonts.getHandle(name);
		if(index != NOT_FOUND)
		{
			const FontInfo& info = _fonts.getFont(index).info;
			if(info.getName().compare(name) == 0 && info.getItalic() == italic && info.getSize() == size && info.getWeight() == weight)
			{
				return index;
			}
			_fonts.Destroy(index); //or throw exception that font with that name already exists
		}
		return _fonts.Insert(LoadFont(name, size, weight, italic));
	}

	//*****************************************************************
	//					GET FONT HANDLE
	//*****************************************************************
	uint32_t DXRenderer::getFontHandle(const char* name)
	{
		return _fonts.getHandle(name);
	}

	//*****************************************************************
	//					GET FONT INFO
	//*****************************************************************
	const FontInfo& DXRenderer::getFontInfo(uint32_t handle) const
	{
		return _fonts.getFont(handle).info;
	}

	//*****************************************************************
	//					LOAD FONT
	//*****************************************************************
	DXFont DXRenderer::LoadFont(const char* name, uint32_t size, uint32_t weight, bool italic) const
	{
		LPD3DXFONT font = nullptr;
		HRESULT result = D3DXCreateFont(_d3ddev, size, 0, weight, 1, italic, DEFAULT_CHARSET,
			OUT_CHARACTER_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, TEXT(name), &font);

		if(FAILED(result))
		{
			//std::exception((std::string("Failed to create font '") + name + "'!").c_str())
			std::exception e;
			throw e;
		}
		
		return DXFont(name, size, italic, weight, font);
	}

	//*****************************************************************
	//					BEGIN SCENE
	//*****************************************************************
	bool DXRenderer::BeginScene()
	{
		HRESULT result = _d3ddev->TestCooperativeLevel();
		if(result == D3DERR_DEVICELOST)
			return false;
		if(result == D3DERR_DEVICENOTRESET)
		{
			ResetDevices();
			return false;
		}
		if(SUCCEEDED(result) && SUCCEEDED(_d3ddev->BeginScene()))
		{
			//_d3ddev->ColorFill(_backbuffer, nullptr, _backgroundFillColor);
			_d3ddev->Clear(0, nullptr, D3DCLEAR_TARGET, _backgroundFillColor, 1.0f, 0);
			_d3ddev->Clear(0, nullptr, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
			return true;
		}
		return false;
	}
	
	//*****************************************************************
	//					END SCENE
	//*****************************************************************
	void DXRenderer::EndScene()
	{
		_d3ddev->EndScene();
		_d3ddev->Present(nullptr, nullptr, nullptr, nullptr);
	}

	//*****************************************************************
	//					BEGIN SPRITE BATCH
	//*****************************************************************
	bool DXRenderer::BeginSpriteBatch(bool alphablend)
	{
		HRESULT result = _spriteHandler->Begin(alphablend ? D3DXSPRITE_ALPHABLEND : 0);
		return SUCCEEDED(result);
	}

	//*****************************************************************
	//					END SPRITE BATCH
	//*****************************************************************
	void DXRenderer::EndSpriteBatch()
	{
		_spriteHandler->End();
	}

	//*****************************************************************
	//					SET TRANSFORM 2D
	//*****************************************************************
	void DXRenderer::setTransform2D(const Util::Vec2* translation, const Util::Vec2* scalingCenter, const Util::Vec2* scale, const Util::Vec2* rotationPivot, float rotationRadians, const Util::Color* colorTint)
	{
		_tintColor = colorTint ? colorTint->getARGB() : D3DCOLOR_XRGB(255,255,255);
		auto sc = MakeVECTOR(*scalingCenter);
		auto s = MakeVECTOR(*scale);
		auto rp = MakeVECTOR(*rotationPivot);
		auto t = MakeVECTOR(*translation);

		D3DXMatrixTransformation2D(&_transformMatrix,
			scalingCenter ? &sc : nullptr,
			0,
			scale ? &s : nullptr,
			rotationPivot ? &rp : nullptr,
			rotationRadians,
			translation ? &t : nullptr);
	}

	//*****************************************************************
	//					SET TRANSFORM 3D
	//*****************************************************************
	void DXRenderer::setTransform3D(const Util::Vec3* translation, const Util::Vec3* scalingCenter, const Util::Vec3* scale, const Util::Vec3* rotationPivot, const Util::Vec3* rotRad, const Util::Color* colorTint)
	{
		
	}
	
	//*****************************************************************
	//					SET FONT STYLE
	//*****************************************************************
	void DXRenderer::setFontStyle(bool noClip, bool singleLine, bool hCenter, bool right, bool vCenter, bool bottom)
	{
		_fontStyle =	noClip	   ? DT_NOCLIP	: 0 |
						hCenter    ? DT_CENTER	: 0 |
						right	   ? DT_RIGHT	: 0 |
						vCenter    ? DT_VCENTER : 0 |
						bottom	   ? DT_BOTTOM	: 0 |
						singleLine ? DT_SINGLELINE : DT_WORDBREAK;
	}

	//*****************************************************************
	//					DRAW POINT
	//*****************************************************************
	void DXRenderer::DrawPoint(const Util::Vec2& pos, const Util::Color* color, float lineWidth)
	{
		auto rgb = color ? color->getARGB() : D3DCOLOR_ARGB(255,255,255,255);
		Vertex vertices[] = {
			{pos.x, pos.y, 1.0f, rgb}
		};
		DrawPrimitive(vertices, 1, DP_Type::LineList, 1);
	}

	//*****************************************************************
	//					DRAW LINE
	//*****************************************************************
	void DXRenderer::DrawLine(const Util::Vec2& start, const Util::Vec2& finish, const Util::Color* color, float lineWidth)
	{
		auto rgb = color ? color->getARGB() : D3DCOLOR_ARGB(255,255,255,255);
		Vertex vertices[] = {
			{start.x, start.y, 1.0f, rgb},
			{finish.x, finish.y, 1.0f, rgb}
		};
		DrawPrimitive(vertices, 2, DP_Type::LineList, 1);
	}

	//*****************************************************************
	//					DRAW TRIANGLE
	//*****************************************************************
	void DXRenderer::DrawTriangle(const Util::Vec2& a, const Util::Vec2& b, const Util::Vec2& c, const Util::Color* color, float lineWidth)
	{
		auto rgb = color ? color->getARGB() : D3DCOLOR_ARGB(255,255,255,255);
		Vertex vertices[] = {
			{a.x, a.y, 1.0f, rgb},
			{b.x, b.y, 1.0f, rgb},
			{c.x, c.y, 1.0f, rgb}
		};
		_vertexFormat = D3DFVF_DIFFUSE|D3DFVF_XYZ;
		DrawPrimitive(vertices, 3, DP_Type::TriangleList, 1);
	}

	//*****************************************************************
	//					DRAW RECTANGLE
	//*****************************************************************
	void DXRenderer::DrawRectangle(const Util::Vec2& pos, uint32_t hw, uint32_t hh, const Util::Color* color, float lineWidth)
	{
		DrawRectangle(Util::Rect(pos, (float)hw*2, (float)hh*2), color, lineWidth);
	}

	void DXRenderer::DrawRectangle(const Util::Rect& rect, const Util::Color* color, float lineWidth)
	{
		auto rgb = color ? color->getARGB() : D3DCOLOR_ARGB(255,255,255,255);
		Vertex vertices[] = {
			{rect.Left(), rect.Top(), 1.0f, rgb},
			{rect.Right(), rect.Top(), 1.0f, rgb},
			{rect.Left(), rect.Bottom(), 1.0f, rgb},
			{rect.Right(), rect.Bottom(), 1.0f, rgb}
		};
		DrawPrimitive(vertices, 4, DP_Type::TriangleStrip, 2);
	}

	//*****************************************************************
	//					DRAW CIRCLE
	//*****************************************************************
	void DXRenderer::DrawCircle(const Util::Vec2& pos, float radius, const Util::Color* color, float lineWidth)
	{
		DrawElipse(pos, radius, radius, color, lineWidth);
	}

	//*****************************************************************
	//					DRAW ELIPSE
	//*****************************************************************
	void DXRenderer::DrawElipse(const Util::Vec2& pos, float xRadius, float yRadius, const Util::Color* color, float lineWidth)
	{
		auto rgb = color ? color->getARGB() : D3DCOLOR_ARGB(255,255,255,255);
		const int numVertices = (int) (std::max(36*0.25, std::ceil((std::min(xRadius, yRadius)*0.5)*0.25)));
		std::vector<Vertex> elipse;
		std::vector<D3DXVECTOR2> quarterElipse;
		elipse.reserve(numVertices*4 + 1);
		quarterElipse.reserve(numVertices);
		float angle = 360.0f/(numVertices*4);

		//center of the elipse
		Vertex vertex = {pos.x, pos.y, 1.0f, rgb};
		//elipse.push_back(vertex); 

		//get the upper right quarter of the elipse
		for(int i = 0; i < numVertices; ++i)
		{
			quarterElipse.push_back(D3DXVECTOR2((std::cos(D3DXToRadian(angle*i))), (std::sin(D3DXToRadian(angle*i)))));

			vertex.x = pos.x + xRadius*quarterElipse.back().x;
			vertex.y = pos.y + yRadius*quarterElipse.back().y;
			elipse.push_back(vertex);

			//circle.push_back(D3DXVECTOR2(pos.x + xRadius*quarterCircle[i].x, pos.y + yRadius*quarterCircle[i].y));
		}

		//get the upper left quarter of the circle
		for(int i = 0; i < numVertices; ++i)
		{
			vertex.x = pos.x - xRadius*quarterElipse[i].y;
			vertex.y = pos.y + yRadius*quarterElipse[i].x;
			elipse.push_back(vertex);
			//circle.push_back(D3DXVECTOR2(pos.x - xRadius*quarterCircle[i].y, pos.y + yRadius*quarterCircle[i].x));
		}
		
		//get the lower left quarter of the circle
		for(int i = 0; i < numVertices; ++i)
		{
			vertex.x = pos.x - xRadius*quarterElipse[i].x;
			vertex.y = pos.y - yRadius*quarterElipse[i].y;
			elipse.push_back(vertex);
			//circle.push_back(D3DXVECTOR2(pos.x - xRadius*quarterCircle[i].x, pos.y - yRadius*quarterCircle[i].y));
		}

		//get the lower right quarter of the circle
		for(int i = 0; i < numVertices; ++i)
		{
			vertex.x = pos.x + xRadius*quarterElipse[i].y;
			vertex.y = pos.y - yRadius*quarterElipse[i].x;
			elipse.push_back(vertex);
			//circle.push_back(D3DXVECTOR2(pos.x + xRadius*quarterCircle[i].y, pos.y - yRadius*quarterCircle[i].x));
		}
		
		DrawPrimitive(&elipse[0], elipse.size(), DP_Type::TriangleFan, elipse.size()-1);
	}
	
	//*****************************************************************
	//					DRAW TEXTURE
	//*****************************************************************
	void DXRenderer::DrawTexture(const TextureData& texture)
	{
		const DXTexture& dxtexture = static_cast<const DXTexture&>(texture);
		if(dxtexture.lpd3dTexture == nullptr)
		{
			return;
		}
		auto source = MakeRECT(Util::Rect(0,0,float(texture.width), float(texture.height)));
		_spriteHandler->SetTransform(&_transformMatrix);
		_spriteHandler->Draw(dxtexture.lpd3dTexture, &source, nullptr, nullptr, _tintColor);
		_spriteHandler->SetTransform(D3DXMatrixIdentity(&_transformMatrix));
	}

	//*****************************************************************
	//					DRAW SPRITE
	//*****************************************************************
	void DXRenderer::DrawSprite(const TextureData& texture, const SpriteInfo& sprite)
	{
		const DXTexture& dxtexture = static_cast<const DXTexture&>(texture);
		if(dxtexture.lpd3dTexture == nullptr)
		{
			return;
		}

		auto source = MakeRECT(sprite.getSrcRect());
		_spriteHandler->SetTransform(&_transformMatrix);
		_spriteHandler->Draw(dxtexture.lpd3dTexture, &source, nullptr, nullptr, _tintColor);
		_spriteHandler->SetTransform(D3DXMatrixIdentity(&_transformMatrix));
	}
	
	//*****************************************************************
	//					DRAW FONT
	//*****************************************************************
	void DXRenderer::DrawFont(uint32_t hFont, const char* text, const Util::Rect* bounds)
	{
		if(!_fonts.Valid(hFont))
		{
			//potentionally throw exception, maybe log the error, or maybe write with a default font
			return;
		}
		const DXFont& font = _fonts.getFont(hFont);
		auto rect = MakeRECT(*bounds);
		_spriteHandler->SetTransform(&_transformMatrix);
		font.data->DrawText(_spriteHandler, text, -1, bounds == nullptr ? nullptr : &rect, _fontStyle, _tintColor);
		_spriteHandler->SetTransform(D3DXMatrixIdentity(&_transformMatrix));
	}

	//*****************************************************************
	//					DRAW VERTEX BUFFER
	//*****************************************************************
	void DXRenderer::DrawPrimitive(const Vertex* buffer, uint32_t numVertices, DP_Type type, uint32_t numPrimitives)
	{
		LPDIRECT3DVERTEXBUFFER9 vbuffer;
		_d3ddev->CreateVertexBuffer(numVertices*sizeof(Vertex), 0, _vertexFormat, D3DPOOL_MANAGED, &vbuffer, nullptr);
		
		VOID* pvoid = nullptr;
		vbuffer->Lock(0, 0, &pvoid, 0);
		memcpy(pvoid, buffer, sizeof(Vertex)*numVertices);
		vbuffer->Unlock();
		
		_d3ddev->SetFVF(_vertexFormat);
		_d3ddev->SetStreamSource(0, vbuffer, 0, sizeof(Vertex));
		
		D3DPRIMITIVETYPE types[] = {D3DPT_POINTLIST, D3DPT_LINELIST, D3DPT_LINESTRIP, D3DPT_TRIANGLELIST, D3DPT_TRIANGLESTRIP, D3DPT_TRIANGLEFAN};
		_d3ddev->DrawPrimitive(types[(int)type], 0, numPrimitives);
		D3DXMatrixIdentity(&_transformMatrix);
		
		vbuffer->Release();
		
	}

	void DXRenderer::DrawIndexedPrimitive(const Vertex* vertexBuffer, uint32_t numVertices, const uint16_t* indexBuffer, uint32_t numIndices, DP_Type drawType, uint32_t numPrimitives)
	{
		LPDIRECT3DVERTEXBUFFER9 vbuffer;
		_d3ddev->CreateVertexBuffer(numVertices*sizeof(Vertex), 0, _vertexFormat, D3DPOOL_MANAGED, &vbuffer, nullptr);
		VOID* pvoid = nullptr;
		vbuffer->Lock(0, 0, &pvoid, 0);
		memcpy(pvoid, vertexBuffer, sizeof(Vertex)*numVertices);
		vbuffer->Unlock();
		_d3ddev->SetStreamSource(0, vbuffer, 0, sizeof(Vertex));
		
		LPDIRECT3DINDEXBUFFER9 ibuffer;
		_d3ddev->CreateIndexBuffer(numIndices*sizeof(uint16_t), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &ibuffer, nullptr);
		ibuffer->Lock(0, 0, &pvoid, 0);
		memcpy(pvoid, indexBuffer, sizeof(uint16_t)*numIndices);
		ibuffer->Unlock();
		_d3ddev->SetIndices(ibuffer);

		_d3ddev->SetFVF(_vertexFormat);

		D3DPRIMITIVETYPE types[] = {D3DPT_POINTLIST, D3DPT_LINELIST, D3DPT_LINESTRIP, D3DPT_TRIANGLELIST, D3DPT_TRIANGLESTRIP, D3DPT_TRIANGLEFAN};
		_d3ddev->DrawIndexedPrimitive(types[(int)drawType], 0, 0, numVertices, 0, numPrimitives);
		D3DXMatrixIdentity(&_transformMatrix);
		
		vbuffer->Release();
		ibuffer->Release();
	}


	/*
	bool DXRenderer::Create(const std::string& name, const uint32_t size, const uint32_t weight, const bool italic, FontStore::TypePtr& out)
	{
		shared_ptr<DXFont> font(new DXFont(name));
		HRESULT result = D3DXCreateFont(_d3ddev, size, 0, weight, 1, italic, DEFAULT_CHARSET,
			OUT_CHARACTER_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, TEXT(name.c_str()), &font->font);

		if(FAILED(result))
		{
			return false;
		}
		
		out = font;
		return true;
	}

	bool DXRenderer::Create(const std::string& filename, TextureStore::TypePtr& out)
	{
		shared_ptr<DXTexture> texture(new DXTexture(filename));
		
		//the struct for reading bitmap file info
		D3DXIMAGE_INFO info;
		HRESULT result;

		if(texture->source == Source_File)
		{
			//create the new texture by loading a bitmap image file
			result = D3DXGetImageInfoFromFile(texture->filename.c_str(), &info);
			if(FAILED(result))
				return false;
			result = D3DXCreateTextureFromFileEx(
				_d3ddev,					//direct3D device object
				texture->filename.c_str(),	//the image filename
				info.Width,					//the image width
				info.Height,				//the image height
				1,							//mip-map levels (1 for no chain)
				D3DPOOL_DEFAULT,			//the type of surface (default)
				D3DFMT_UNKNOWN,				//texture format
				D3DPOOL_MANAGED,			//memory class for the image
				D3DX_DEFAULT,				//image filter
				D3DX_DEFAULT,				//mip filter
				_transparentColor,			//color key for transparency
				&info,						//bitmap file info (from loaded file)
				nullptr,						//color palette
				&texture->texture);			//destination texture
		}
		else if(texture->source == Source_Resource)
		{
			result = D3DXGetImageInfoFromResource(nullptr, texture->filename.c_str(), &info);
			if(FAILED(result))
				return false;
			result = D3DXCreateTextureFromResourceEx(
				_d3ddev,					//direct3D device object
				nullptr,						//module
				texture->filename.c_str(),	//the image filename
				info.Width,					//the image width
				info.Height,				//the image height
				1,							//mip-map levels (1 for no chain)
				D3DPOOL_DEFAULT,			//the type of surface (default)
				D3DFMT_UNKNOWN,				//texture format
				D3DPOOL_MANAGED,			//memory class for the image
				D3DX_DEFAULT,				//image filter
				D3DX_DEFAULT,				//mip filter
				_transparentColor,			//color key for transparency
				&info,						//bitmap file info (from loaded file)
				nullptr,						//color palette
				&texture->texture);			//destination texture
		}

		texture->width = info.Width;
		texture->height = info.Height;

		if(FAILED(result))
		{
			return false;
		}

		texture->ParseDescriptor();
		out = texture;
		return true;
	}

	void DXRenderer::LoadGraphics(const std::string& resourceFile)
	{
		if(resourceFile.empty())
			return;
		//tiny xml loading
		ticpp::Document doc(resourceFile);
		try
		{
			doc.LoadFile();

			ticpp::Iterator< ticpp::Element > resource;
			for( resource = resource.begin( doc.FirstChildElement() ); resource != resource.end(); ++resource )
			{
				std::string resourceType;
				resource->GetValue(&resourceType);
				if(resourceType == "File")
				{
					std::string path;
					resource->GetText(&path);
					LoadGraphics(path);
				}
				else if(resourceType == "TextureResource")
				{
					std::string filename, alias;
					resource->GetText(&filename);
					resource->GetAttributeOrDefault("alias", &alias, filename);
					LoadTexture(filename, alias);
				}
				else if(resourceType == "FontResource")
				{
					std::string alias, font;
					bool italic;
					uint32_t size, weight;
					
					resource->GetText(&font);
					resource->GetAttributeOrDefault("alias", &alias, font);
					resource->GetAttribute("weight", &weight);
					resource->GetAttribute("italic", &italic);
					resource->GetAttribute("size", &size);
					LoadFont(font, size, weight, italic, alias);
				}
			}
		}
		catch(ticpp::Exception& e)
		{
			Logger() << e.what() << Logger::endline;
		}
	}


	bool DXRenderer::setTranslation(const Util::Vec3& translation)
	{
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, translation.x, translation.y, translation.z);
		_transformMatrix *= mat;
		return true;
	}

	bool DXRenderer::setScaling(const Util::Vec3& scale)
	{
		D3DXMATRIX mat;
		D3DXMatrixScaling(&mat, scale.x, scale.y, scale.z);
		_transformMatrix *= mat;
		return true;
	}

	bool DXRenderer::setRotation(const Util::Vec3& rotation)
	{
		D3DXMATRIX mat;
		D3DXMatrixRotationYawPitchRoll(&mat, rotation.y, rotation.x, rotation.z);
		_transformMatrix *= mat;
		return true;
	}

	bool DXRenderer::setProjectionMatrix(float vertFovDeg, float nearViewPlane, float farViewPlane, float screenAspectRatio)
	{
		D3DXMATRIX mat;
		if(screenAspectRatio < 0.1f)
		{
			auto screen = getScreenSize();
			screenAspectRatio = screen.x/screen.y;
		}
		D3DXMatrixPerspectiveFovLH(&mat, D3DXToRadian(vertFovDeg), screenAspectRatio, nearViewPlane, farViewPlane);
		return SUCCEEDED(_d3ddev->SetTransform(D3DTS_PROJECTION, &mat));
	}

	bool DXRenderer::setViewMatrix(const Util::Vec3& cameraPosition, const Util::Vec3& cameraLookAt, const Util::Vec3& cameraUpVector)
	{
		D3DXMATRIX mat;
		auto cp = MakeVECTOR(cameraPosition);
		auto cla = MakeVECTOR(cameraLookAt);
		auto cuv = MakeVECTOR(cameraUpVector);
		D3DXMatrixLookAtLH(&mat, &cp, &cla, &cuv);
		return SUCCEEDED(_d3ddev->SetTransform(D3DTS_VIEW, &mat));
	}

	bool DXRenderer::setWorldTransformMatrix()
	{
		return SUCCEEDED(_d3ddev->SetTransform(D3DTS_WORLD, &_transformMatrix));
	}

	bool DXRenderer::appendFVF(FVF_Type format)
	{
		uint32_t fvfs[] = { D3DFVF_XYZ, D3DFVF_XYZRHW, D3DFVF_NORMAL, D3DFVF_DIFFUSE, D3DFVF_SPECULAR, D3DFVF_TEX0, D3DFVF_TEX1, D3DFVF_TEX2, D3DFVF_TEX3, D3DFVF_TEX4, D3DFVF_TEX5, D3DFVF_TEX6, D3DFVF_TEX7, D3DFVF_TEX8 };
		_vertexFormat |= fvfs[(uint32_t)format];
		return true;
	}

	bool DXRenderer::clearFVF()
	{
		_vertexFormat = 0;
		return true;
	}

	bool DXRenderer::applyFVF(uint32_t format)
	{
		return SUCCEEDED(_d3ddev->SetFVF(format == 0 ? _vertexFormat : format));
	}

	uint32_t DXRenderer::getFVF() const
	{
		return _vertexFormat;
	}

	bool DXRenderer::setRenderStateLighting(bool enabled)
	{
		return SUCCEEDED(_d3ddev->SetRenderState(D3DRS_LIGHTING, enabled));
	}

	bool DXRenderer::setRenderStateZBuffer(bool enabled)
	{
		return SUCCEEDED(_d3ddev->SetRenderState(D3DRS_ZENABLE, enabled));
	}

	bool DXRenderer::setRenderStateCulling(RS_Culling cullmode)
	{
		DWORD modes[] = {D3DCULL_NONE, D3DCULL_CW, D3DCULL_CCW};
		return SUCCEEDED(_d3ddev->SetRenderState(D3DRS_CULLMODE, modes[(int)cullmode]));
	}

	bool DXRenderer::setRenderStateFillmode(RS_Fillmode fillmode)
	{
		DWORD modes[] = {D3DFILL_POINT, D3DFILL_WIREFRAME, D3DFILL_SOLID};
		return SUCCEEDED(_d3ddev->SetRenderState(D3DRS_FILLMODE, modes[(int)fillmode]));
	}
*/
}
