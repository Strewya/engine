//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** C++ headers ***/
#define WIN32_LEAN_AND_MEAN 1
#include <cassert>
#include <string>
#include <windows.h>
	/*** extra headers ***/
#include "Defines.h"
#include "Win32/GameWindow.h"
#include "Win32/WindowClass.h"
//---------- testing purposes
#include "Core/Entity/Entity.h"
#include "Core/Form/Form.h"
#include "Core/State/State.h"
#include "Core/State/Statemap.h"
#include "Subsystems/Graphics/Interface.h"
#include "Subsystems/Graphics/RendererFactory.h"
#include "Subsystems/Graphics/RenderingQueue.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Graphics/TextureInfo.h"
#include "Subsystems/Script/LuaEngine.h"
#include "Util/Color.h"
#include "Util/Dimensional.h"
	/*** end headers ***/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	String className = "Snake";
	Win32::WindowClass wndClass(className);
	
	if(wndClass.Register() == 0)
	{
		MessageBox(nullptr, "WindowClass::Register(): Failed to register the window class.", "Initialization error", MB_OK);
		return ErrorCode::WindowClassRegistration;
	}

	Win32::GameWindow gameWindow(wndClass.getClassName(), wndClass.getClassName());
	gameWindow.setFullscreen(false);
	gameWindow.setSize(gameWindow.getSizeX()-30, gameWindow.getSizeY()-120);
	
	if(!gameWindow.Create())
	{
		MessageBoxA(nullptr, "Window::Create(): Failed to create a window.", "Initialization error", MB_OK);
		return ErrorCode::WindowCreation;
	}

	gameWindow.Show();

	try
	{
		Graphics::RendererFactory factory(hInst, gameWindow.getWindowHandle());
		factory.InitInterface("dx");
		auto gfx = factory.getInterface();
		gfx->setScreenSize(gameWindow.getSizeX(), gameWindow.getSizeY());

		Graphics::SpritesheetCache sheetCache;
		Graphics::Spritesheet& sheet = sheetCache.CreateFromFile("resources/pong.sheet");
		uint ballHandle = sheet.getHandle("ball");
		uint cherryHandle = sheet.getHandle("cherry");
		uint cupcakeHandle = sheet.getHandle("cupcake");

		Core::Entity ent(0);
		ent.getForm().setType(Core::FormType::Sprite);

		uint tex = gfx->getTextureHandle(sheet.getTextureName());
		sheet.setTextureHandle(tex);
		const Graphics::TextureInfo& texinfo = gfx->getTextureInfo(tex);

		std::vector<Core::Form> sprites;
		Core::State sheetState(sheet);
		//cherry
		sprites.push_back(Core::Form(Core::FormType::Sprite));
		sprites.back().getStates().Insert("Spritesheet", sheetState);
		sprites.back().getStates().Insert("CurrentSprite", cherryHandle);
		sprites.back().setPosition(500,300);
		sprites.back().setScale(1,1);
		sprites.back().setScalingCenter(sheet.getSprite(cherryHandle).getSrcRect().GetSize()/2);
		//sprites.back().setScalingCenter(sheet.getSprite(cherryHandle).getSrcRect().GetSize());
		//sprites.back().setScalingCenter(Util::Vec2(0,0));
		sprites.back().setPivotPoint(sheet.getSprite(cherryHandle).getSrcRect().GetSize()/2);
		//sprites.back().setPivotPoint(sheet.getSprite(cherryHandle).getSrcRect().GetSize());
		//sprites.back().setPivotPoint(Util::Vec2(0,0));
		sprites.back().setRotation(0);

		//point at (0,0) of cherry texture
		sprites.push_back(Core::Form(Core::FormType::Sprite));
		sprites.back().getStates().Insert("Spritesheet", sheetState);
		sprites.back().getStates().Insert("CurrentSprite", ballHandle);
		sprites.back().setPosition(Util::Vec2(500,300) - sheet.getSprite(cherryHandle).getSrcRect().GetSize()/2);
		sprites.back().setScale(0.1f,0.1f);
		sprites.back().setScalingCenter(sheet.getSprite(ballHandle).getSrcRect().GetSize()/2);
		sprites.back().setPivotPoint(sheet.getSprite(ballHandle).getSrcRect().GetSize()/2);
		sprites.back().setRotation(0);

		//point at center of cherry texture
		sprites.push_back(Core::Form(Core::FormType::Sprite));
		sprites.back().getStates().Insert("Spritesheet", sheetState);
		sprites.back().getStates().Insert("CurrentSprite", ballHandle);
		sprites.back().setPosition(500,300);
		sprites.back().setScale(0.1f,0.1f);
		sprites.back().setScalingCenter(sheet.getSprite(ballHandle).getSrcRect().GetSize()/2);
		sprites.back().setPivotPoint(sheet.getSprite(ballHandle).getSrcRect().GetSize()/2);
		sprites.back().setRotation(0);

		//point at (w,h) of cherry texture
		sprites.push_back(Core::Form(Core::FormType::Sprite));
		sprites.back().getStates().Insert("Spritesheet", sheetState);
		sprites.back().getStates().Insert("CurrentSprite", ballHandle);
		sprites.back().setPosition(Util::Vec2(500,300) + sheet.getSprite(cherryHandle).getSrcRect().GetSize()/2);
		sprites.back().setScale(0.1f,0.1f);
		sprites.back().setScalingCenter(sheet.getSprite(ballHandle).getSrcRect().GetSize()/2);
		sprites.back().setPivotPoint(sheet.getSprite(ballHandle).getSrcRect().GetSize()/2);
		sprites.back().setRotation(0);

		//font type
		uint hFont = gfx->MakeFont("Arial", 20, 400, false);
		Util::Rect fontBounds(50, 50, 300, 300);
		sprites.push_back(Core::Form(Core::FormType::Font));
		sprites.back().getStates().Insert("Font", hFont);
		sprites.back().getStates().Insert("Area", fontBounds);
		sprites.back().getStates().Insert("Text", String("Determines the width and height of the rectangle. If there are multiple lines of text, DrawText uses the width of the rectangle pointed to by the pRect parameter and extends the base of the rectangle to bound the last line of text. If there is only one line of text, DrawText modifies the right side of the rectangle so that it bounds the last character in the line. In either case, DrawText returns the height of the formatted text but does not draw the text."));


		sprites.push_back(Core::Form(Core::FormType::Triangle));
		sprites.back().getStates().Insert("Point1", Util::Vec2(0,-10));
		sprites.back().getStates().Insert("Point2", Util::Vec2(-10,0));
		sprites.back().getStates().Insert("Point3", Util::Vec2(10,0));
		sprites.back().setPosition(500,500);
		

		sprites.push_back(Core::Form(Core::FormType::Rectangle));
		sprites.back().getStates().Insert("Area", fontBounds);


		Graphics::RenderingQueue opaque;
		Graphics::RenderingQueue alphaBlended;
		
		Util::Timer mainLoopTimer;
	
		float ticksPerSec = 1.0f/40;
		//start main loop
		while(gameWindow.Update())
		{	
			gClock.FrameStep();
			//engine.Update();
			
			//the rest of this code should be inside Engine::Update()

			while( mainLoopTimer.TimeToUpdate(ticksPerSec) )
			{
				for( auto& spr : sprites )
				{
					//spr.setRotation(spr.getRotation()+1);
				}
				//fontRot = ++fontRot >= 360 ? fontRot-360 : fontRot;
			}
			gfx->BeginScene();
			gfx->BeginSpriteBatch(true);
		
			for( auto& form : sprites)
			{
				alphaBlended.Add(&form);
			}
			alphaBlended.Render(*gfx);
			alphaBlended.Clear();
			
			
			gfx->EndSpriteBatch();
			gfx->BeginSpriteBatch(false);

			gfx->DrawRectangle(Util::Vec2(250,400), 30, 30);

			gfx->DrawCircle(Util::Vec2(600,200), 10);
			gfx->DrawCircle(Util::Vec2(700,200), 40);
			gfx->DrawCircle(Util::Vec2(900,200), 100);

			gfx->DrawElipse(Util::Vec2(800,500), 10, 15);
			gfx->DrawElipse(Util::Vec2(800,500), 40, 20);
			gfx->DrawElipse(Util::Vec2(800,500), 100, 90);

			gfx->EndSpriteBatch();
			gfx->EndScene();
		}

		return gameWindow.getExitCode();
	}
	catch(std::exception& ex)
	{
		MessageBox(gameWindow.getWindowHandle(), ex.what(), "Exception error", MB_OK);
		return ErrorCode::ExceptionThrown;
	}
}