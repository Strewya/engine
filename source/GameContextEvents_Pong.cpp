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

		auto& sheet = context.resources.getSpritesheetCache().LoadFromFile("resources/pong.sheet");
		auto& sheet2 = context.resources.getSpritesheetCache().LoadFromFile("resources/character.sheet");
		auto guiHandle = context.resources.getTextureCache().getTextureHandle("resources/gui_window.png");
		auto& gui = context.resources.getTextureCache().getTexture(guiHandle);
		
		uint ballHandle = sheet.getSpriteHandle("ball");
		uint cherryHandle = sheet.getSpriteHandle("cherry");
		uint cupcakeHandle = sheet.getSpriteHandle("cupcake");
		uint paddleHandle = sheet.getSpriteHandle("paddle");

		uint animationHandle = sheet2.getAnimationHandle("walk");

		auto& ent = context.entities.NewInstance();
		auto& frm = ent.getForm();
		frm.setType(Core::FormType::Sprite);
		frm.setPosition(500,300);
		frm.setScale(4,4);
		frm.Insert("Spritesheet", Core::State::Create<Graphics::Spritesheet&>(sheet2));
		frm.Insert("CurrentSprite", Core::State::Create(sheet2.getAnimation(animationHandle).getSequenceFrame(0)));
		frm.Insert("CurrentAnimationFrame", Core::State::Create<uint>(0));
		frm.Insert("CurrentAnimation", Core::State::Create(animationHandle));
		frm.Insert("AnimationTimer", Core::State::Create(Util::Timer(0.5f)));

		ent.Insert("Render", Core::Action(Core::Render));
		assert(ent.getAction("Render").Activate());
		ent.Insert("Animate", Core::Action(Core::Animate));
		assert(ent.getAction("Animate").Activate());
		ent.Insert("SIH", Core::Action(Core::SIH));
		assert(ent.getAction("SIH").Activate());

		auto& ent2 = context.entities.NewInstance();
		auto& frm2 = ent2.getForm();
		frm2.setType(Core::FormType::Texture);
		frm2.setPosition(0,0);

		frm2.Insert("Texture", Core::State::Create(gui));

		ent2.Insert("Render", Core::Action(Core::Render));
		assert(ent2.getAction("Render").Activate());

		assert(context.entities.IsAlive(ent.getID()));
		assert(context.entities.IsAlive(ent2.getID()));

		return true;
	}
}