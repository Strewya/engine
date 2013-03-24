//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameContextEvents_Pong.h"
	/*** C++ headers ***/
#include <memory>
	/*** extra headers ***/
#include "GameContext.h"
#include "Core/Action/Action.h"
#include "Core/Action/Functions.h"
#include "Core/Entity/Entity.h"
#include "Core/Form/Form.h"
#include "Core/State/State.h"
#include "Subsystems/Graphics/Spritesheet.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Script/LuaEngine.h"
	/*** end headers ***/

namespace Pong
{
	bool ContextGameplayCreate(Core::GameContext& context)
	{
		context.actionMaster.EnqueueActionLogic(Core::Rotate);
		context.actionMaster.EnqueueActionLogic(Core::Render);

		Graphics::Spritesheet& sheet = context.resources.getSpritesheetCache().LoadFromFile("resources/pong.sheet");
		Graphics::Spritesheet& sheet2 = context.resources.getSpritesheetCache().LoadFromFile("resources/character.sheet");
		
		uint ballHandle = sheet.getSpriteHandle("ball");
		uint cherryHandle = sheet.getSpriteHandle("cherry");
		uint cupcakeHandle = sheet.getSpriteHandle("cupcake");
		uint paddleHandle = sheet.getSpriteHandle("paddle");

		auto* ent = context.entities.getNewEntity();
		
		ent->getForm().setType(Core::FormType::Sprite);
		ent->getForm().Insert("Spritesheet", Core::IState::Create<String>("pong.sheet"));
		ent->getForm().Insert("CurrentSprite", Core::IState::Create(paddleHandle));
		ent->getForm().setPosition(500,300);
		ent->getForm().setScale(1,1);
		ent->getForm().setScalingCenter(sheet.getSprite(paddleHandle).getSrcRect().GetSize()/2);
		auto pivot = sheet.getSprite(paddleHandle).getSrcRect().GetSize();
		pivot.x *= 0.5f;
		pivot.y *= 0.9f;
		ent->getForm().setPivotPoint(pivot);
		ent->getForm().setRotation(0);

		ent->Insert("Render", Core::Action(Core::Render));
		assert(ent->getAction("Render").Activate());
		ent->Insert("Rotate", Core::Action(Core::Rotate));
		assert(ent->getAction("Rotate").Activate());

		return true;
	}
}