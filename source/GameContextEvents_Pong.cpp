//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameContextEvents_Pong.h"
	/*** C++ headers ***/
#include <memory>
	/*** extra headers ***/
#include "Engine/GameContext.h"
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
		context.actionMaster.EnqueueActionLogic(Core::SIH);
		context.actionMaster.EnqueueActionLogic(Core::Rotate);
		context.actionMaster.EnqueueActionLogic(Core::Animate);
		context.actionMaster.EnqueueActionLogic(Core::Render);

		Graphics::Spritesheet& sheet = context.resources.getSpritesheetCache().LoadFromFile("resources/pong.sheet");
		Graphics::Spritesheet& sheet2 = context.resources.getSpritesheetCache().LoadFromFile("resources/character.sheet");
		
		uint ballHandle = sheet.getSpriteHandle("ball");
		uint cherryHandle = sheet.getSpriteHandle("cherry");
		uint cupcakeHandle = sheet.getSpriteHandle("cupcake");
		uint paddleHandle = sheet.getSpriteHandle("paddle");

		uint animationHandle = sheet2.getAnimationHandle("walk");

		auto* ent = context.entities.getNewEntity();
		auto& frm = ent->getForm();
		frm.setType(Core::FormType::Sprite);
		frm.setPosition(500,300);
		frm.setScale(4,4);
		
		frm.Insert("Spritesheet", Core::IState::Create<String>(sheet2.getSpritesheetName()));
		frm.Insert("CurrentSprite", Core::IState::Create(sheet2.getAnimation(animationHandle).getSequenceFrame(0)));
		frm.Insert("CurrentAnimationFrame", Core::IState::Create<uint>(0));
		frm.Insert("CurrentAnimation", Core::IState::Create(animationHandle));


		ent->Insert("Render", Core::Action(Core::Render));
		assert(ent->getAction("Render").Activate());
		ent->Insert("Animate", Core::Action(Core::Animate));
		assert(ent->getAction("Animate").Activate());
		ent->Insert("SIH", Core::Action(Core::SIH));
		assert(ent->getAction("SIH").Activate());

		return true;
	}
}