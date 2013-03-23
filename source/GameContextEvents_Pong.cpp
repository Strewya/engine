//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameContextEvents_Pong.h"
	/*** C++ headers ***/
#include <memory>
#include <set>
	/*** extra headers ***/
#include "GameContext.h"
#include "Core/Action/Action.h"
#include "Core/Action/Functions.h"
#include "Core/Entity/Entity.h"
#include "Core/Form/Form.h"
#include "Core/State/State.h"
#include "Subsystems/Graphics/Interface.h"
#include "Subsystems/Graphics/Spritesheet.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Graphics/Texture.h"
	/*** end headers ***/

namespace Pong
{
	bool ContextGameplayCreate(Core::GameContext& context)
	{
		context.actionMaster.EnqueueActionLogic(Core::Rotate);
		context.actionMaster.EnqueueActionLogic(Core::Render);

		Graphics::Spritesheet& sheet = context.resources.getSpritesheetCache().CreateFromFile("resources/pong.sheet");
		auto& charSheet = context.resources.getSpritesheetCache().CreateFromFile("resources/character.sheet");
		uint ballHandle = sheet.getHandle("ball");
		uint cherryHandle = sheet.getHandle("cherry");
		uint cupcakeHandle = sheet.getHandle("cupcake");
		uint paddleHandle = sheet.getHandle("paddle");
		Graphics::Texture& tex = context.resources.getTextureCache().getTexture(sheet.getTextureName().c_str());
		sheet.setTexture(tex);

		auto* ent = context.entities.getNewEntity();
		
		ent->getForm().setType(Core::FormType::Sprite);
		ent->getForm().Insert("Spritesheet", Core::IState::Create(sheet));
		ent->getForm().Insert("CurrentSprite", Core::IState::Create(paddleHandle));
		ent->getForm().setPosition(500,300);
		ent->getForm().setScale(1,1);
		ent->getForm().setScalingCenter(sheet.getSprite(paddleHandle).getSrcRect().GetSize()/2);
		ent->getForm().setPivotPoint(sheet.getSprite(paddleHandle).getSrcRect().GetSize()/2);
		ent->getForm().setRotation(0);

		ent->Insert("Render", Core::Action(Core::Render));
		assert(ent->getAction("Render").Activate());


		return true;
	}
}