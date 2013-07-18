//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameContextEvents_Pong.h"
	/*** C++ headers ***/
#include <map>
#include <memory>
#include <vector>
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Core/Action/Action.h"
#include "Core/Action/Impl/ARender.h"
#include "Core/Entity/Entity.h"
#include "Core/State/State.h"
#include "Subsystems/Graphics/IRenderer.h"
#include "Subsystems/Graphics/ITextureCache.h"
#include "Subsystems/Graphics/Spritesheet.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Input/InputEngine.h"
#include "Subsystems/Script/LuaEngine.h"
#include "Util/Color.h"

//temporary
#include "Subsystems/Graphics/Vertex.h"
	/*** end headers ***/

namespace Pong
{
	bool CreatePaddle(Core::GameContext& context, Core::Entity& paddle)
	{
		
		return false;
	}

	bool CreateBall(Core::GameContext& context, Core::Entity& ball)
	{
		
		return false;
	}


	bool onUpdate(Core::GameContext& context)
	{
		auto& gfx = context.services.getGraphics();


		Graphics::Vertex vbuffer[] = {
			{ -3.0f, 3.0f, -3.0f, Util::Color(127, 0, 0, 255).getARGB(), },    // vertex 0
			{ 3.0f, 3.0f, -3.0f, Util::Color(127, 0, 255, 0).getARGB(), },     // vertex 1
			{ -3.0f, -3.0f, -3.0f, Util::Color(127, 255, 0, 0).getARGB(), },   // 2
			{ 3.0f, -3.0f, -3.0f, Util::Color(127, 0, 255, 255).getARGB(), },  // 3
			{ -3.0f, 3.0f, 3.0f, Util::Color(127, 0, 0, 255).getARGB(), },     // ...
			{ 3.0f, 3.0f, 3.0f, Util::Color(127, 255, 0, 0).getARGB(), },
			{ -3.0f, -3.0f, 3.0f, Util::Color(127, 0, 255, 0).getARGB(), },
			{ 3.0f, -3.0f, 3.0f, Util::Color(127, 0, 255, 255).getARGB(), },
		};

		gfx.BeginScene();

		gfx.DrawVertexBuffer(vbuffer, 8, gfx.TriangleList(), 12);


		//gfx.DrawPoint(Util::Vec2(0,0), &Util::Color(0,255,0));
		//gfx.DrawLine(Util::Vec2(0,0), Util::Vec2(2,0), &Util::Color(255,0,255));
		//gfx.DrawLine(Util::Vec2(0,0), Util::Vec2(0,2), &Util::Color(255,0,255));
		//gfx.DrawTriangle(Util::Vec2(470,200), Util::Vec2(500,100), Util::Vec2(600,300), &Util::Color(0,140,40));
		//gfx.DrawRectangle(Util::Vec2(800,400), 120, 80, &Util::Color(180,40, 0));
		//gfx.DrawRectangle(Util::Rect(1000, 600, 200, 100), &Util::Color(70, 40, 20));
		//gfx.DrawCircle(Util::Vec2(320, 600), 150, &Util::Color(0, 40, 200));
		//gfx.DrawElipse(Util::Vec2(1400, 400), 150, 300, &Util::Color(127, 40, 20));

		gfx.EndScene();
		return true;
	}

	bool ContextGameplayCreate(Core::GameContext& context)
	{
		context.setContextEventLogic(Core::GameContext::OnUpdate, onUpdate);

		context.services.getGraphics().setBackgroundFillColor(0,0,127);

		return true;
	}
}