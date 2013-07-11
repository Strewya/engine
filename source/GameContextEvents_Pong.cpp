//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameContextEvents_Pong.h"
	/*** C++ headers ***/
#include <map>
#include <memory>
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Core/Action/Action.h"
#include "Core/Action/Functions.h"
#include "Core/Entity/Entity.h"
#include "Core/State/State.h"
#include "Subsystems/Graphics/IRenderer.h"
#include "Subsystems/Graphics/ITextureCache.h"
#include "Subsystems/Graphics/Spritesheet.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Input/InputEngine.h"
#include "Subsystems/Script/LuaEngine.h"
#include "Util/Color.h"
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

	bool ContextGameplayCreate(Core::GameContext& context)
	{
		Core::Entity* e = nullptr;
		auto eid = context.entityPool.getNewInstance(&e);

		InstanceID hPong = context.resources.getTextureCache().loadTexture("../resources/pong.png", Util::Color(255,0,255), nullptr);

		e->insert(Core::TextureHandle::create());
		e->insert(Core::Triangle::create(Util::Vec2(1,1), Util::Vec2(1,0), Util::Vec2(0,1)));

		auto* sTexture = e->getState<Core::TextureHandle>();

		auto* sTriangle = e->getState<Core::Triangle>();
		sTriangle->a.x = 2;

		auto klon = sTriangle->typeClone();
		klon->a.y = 42;

		return true;
	}
}