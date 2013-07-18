//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/DirectX/DXRenderer.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
#include "Subsystems/Graphics/TextureData.h"
#include "Subsystems/Graphics/Vertex.h"
#include "Util/Logger.h"
	/*** end headers ***/

namespace Graphics
{
	DXRenderer::DXRenderer(HWND hwnd)
		: _transparentColor(D3DCOLOR_XRGB(255,0,255)), _backgroundFillColor(D3DCOLOR_XRGB(0,0,0)), _hwnd(hwnd), _d3d(nullptr), _d3ddev(nullptr), _backbuffer(nullptr),
		_spriteHandler(nullptr), _line(nullptr), _vertexFormat(D3DFVF_XYZ | D3DFVF_DIFFUSE)
	{
		if(!InitDevices())
		{
			throw std::exception("DirectX graphics subsystem has failed to load.");
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
		RECT rc = {0,0, _d3dpp.BackBufferWidth, _d3dpp.BackBufferHeight };
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
			throw std::exception((String("Failed to create font '") + name + "'!").c_str());
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

		D3DXMatrixTransformation2D(&_transformMatrix,
			scalingCenter ? &MakeVECTOR(*scalingCenter) : nullptr,
			0,
			scale ? &MakeVECTOR(*scale) : nullptr,
			rotationPivot ? &MakeVECTOR(*rotationPivot) : nullptr,
			rotationRadians,
			translation ? &MakeVECTOR(*translation) : nullptr);
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
		DrawVertexBuffer(vertices, 1, D3DPT_POINTLIST, 1);
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
		DrawVertexBuffer(vertices, 2, D3DPT_LINELIST, 1);
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
		DrawVertexBuffer(vertices, 3, D3DPT_TRIANGLELIST, 1);
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
		DrawVertexBuffer(vertices, 4, D3DPT_TRIANGLESTRIP, 2);
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
		const int numVertices = (int) (max(36*0.25, std::ceil((min(xRadius, yRadius)*0.5)*0.25)  ));
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
		
		DrawVertexBuffer(&elipse[0], elipse.size(), D3DPT_TRIANGLEFAN, elipse.size()-1);
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
		Util::Rect source(0,0,float(texture.width), float(texture.height));
		_spriteHandler->SetTransform(&_transformMatrix);
		_spriteHandler->Draw(dxtexture.lpd3dTexture, &MakeRECT(source), nullptr, nullptr, _tintColor);
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
		Util::Rect source = sprite.getSrcRect();
		_spriteHandler->SetTransform(&_transformMatrix);
		_spriteHandler->Draw(dxtexture.lpd3dTexture, &MakeRECT(source), nullptr, nullptr, _tintColor);
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
		
		_spriteHandler->SetTransform(&_transformMatrix);
		font.data->DrawText(_spriteHandler, text, -1, bounds == nullptr ? nullptr : &MakeRECT(*bounds), _fontStyle, _tintColor);
		_spriteHandler->SetTransform(D3DXMatrixIdentity(&_transformMatrix));
	}

	//*****************************************************************
	//					DRAW VERTEX BUFFER
	//*****************************************************************
	void DXRenderer::DrawVertexBuffer(const Vertex* buffer, uint32_t numVertices, uint32_t type, uint32_t numPrimitives)
	{
		LPDIRECT3DVERTEXBUFFER9 vbuffer;
		_d3ddev->CreateVertexBuffer(numVertices*sizeof(Vertex), 0, _vertexFormat, D3DPOOL_MANAGED, &vbuffer, nullptr);
		
		
		VOID* pvoid = nullptr;
		vbuffer->Lock(0, 0, &pvoid, 0);
		memcpy(pvoid, buffer, sizeof(Vertex)*numVertices);
		vbuffer->Unlock();
		
		_d3ddev->SetFVF(_vertexFormat);
		_d3ddev->SetStreamSource(0, vbuffer, 0, sizeof(Vertex));
		
		short indices[] =
		{
			0, 1, 2,    // side 1
			2, 1, 3,
			4, 0, 6,    // side 2
			6, 0, 2,
			7, 5, 6,    // side 3
			6, 5, 4,
			3, 1, 7,    // side 4
			7, 1, 5,
			4, 5, 0,    // side 5
			0, 5, 1,
			3, 7, 2,    // side 6
			2, 7, 6,
		};
		LPDIRECT3DINDEXBUFFER9 i_buffer;
		_d3ddev->CreateIndexBuffer(36*sizeof(short), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &i_buffer, nullptr);

		i_buffer->Lock(0, sizeof(indices), &pvoid, 0);
		memcpy(pvoid, indices, sizeof(indices));
		i_buffer->Unlock();
		_d3ddev->SetIndices(i_buffer);

		D3DXVECTOR3 vy[] = {
			D3DXVECTOR3(3,3,3),
			D3DXVECTOR3(3,3,-3),
			D3DXVECTOR3(3,-3,-3),
			D3DXVECTOR3(3,-3,3),
			D3DXVECTOR3(-3,-3,3),
			D3DXVECTOR3(-3,3,3),
			D3DXVECTOR3(-3,3,-3),
			D3DXVECTOR3(-3,-3,-3),
			D3DXVECTOR3(-3,-3,3),
			D3DXVECTOR3(3,-3,3),
			D3DXVECTOR3(3,3,3),
			D3DXVECTOR3(-3,3,3),
			D3DXVECTOR3(-3,3,-3),
			D3DXVECTOR3(3,3,-3),
			D3DXVECTOR3(3,-3,-3),
			D3DXVECTOR3(-3,-3,-3),
		};

		
		


		static float i = 0.0f;
		i += 0.01f;
		D3DXMATRIX roty;
		D3DXMatrixRotationY(&roty, i);
		_d3ddev->SetTransform(D3DTS_WORLD, &(roty));
		

		D3DXMATRIX matView;
		D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(10,10,-10), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
		_d3ddev->SetTransform(D3DTS_VIEW, &matView);

		D3DXMATRIX matProj;
		auto screen = getScreenSize();
		D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45), screen.x/screen.y, 1.0f, 100.0f);
		_d3ddev->SetTransform(D3DTS_PROJECTION, &matProj);
		

		_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
		_d3ddev->SetRenderState(D3DRS_ZENABLE, true);
		_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		

		_d3ddev->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type, 0, 0, numVertices, 0, numPrimitives);

		
		vbuffer->Release();
		i_buffer->Release();

		D3DXMATRIX ln;
		D3DXMatrixIdentity(&ln);

		_line->SetWidth(2);
		_line->Begin();
		
		_line->DrawTransform(vy, sizeof(vy)/sizeof(D3DXVECTOR3), &(roty*matView*matProj), 0xffffffff);
		_line->End();
		
	}

	uint32_t DXRenderer::PointList() const
	{return D3DPT_POINTLIST;}

	uint32_t DXRenderer::LineList() const
	{return D3DPT_LINELIST;}

	uint32_t DXRenderer::LineStrip() const
	{return D3DPT_LINESTRIP;}

	uint32_t DXRenderer::TriangleList() const
	{return D3DPT_TRIANGLELIST;}

	uint32_t DXRenderer::TriangleStrip() const
	{return D3DPT_TRIANGLESTRIP;}

	uint32_t DXRenderer::TriangleFan() const
	{return D3DPT_TRIANGLEFAN;}


	/*
	bool DXRenderer::Create(const String& name, const uint32_t size, const uint32_t weight, const bool italic, FontStore::TypePtr& out)
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

	bool DXRenderer::Create(const String& filename, TextureStore::TypePtr& out)
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

	void DXRenderer::LoadGraphics(const String& resourceFile)
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
				String resourceType;
				resource->GetValue(&resourceType);
				if(resourceType == "File")
				{
					String path;
					resource->GetText(&path);
					LoadGraphics(path);
				}
				else if(resourceType == "TextureResource")
				{
					String filename, alias;
					resource->GetText(&filename);
					resource->GetAttributeOrDefault("alias", &alias, filename);
					LoadTexture(filename, alias);
				}
				else if(resourceType == "FontResource")
				{
					String alias, font;
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
	*/
}