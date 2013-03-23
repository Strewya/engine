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
		context.actionMaster.EnqueueActionLogic(Core::InfinityPattern);
		context.actionMaster.EnqueueActionLogic(Core::Rotate);
		context.actionMaster.EnqueueActionLogic(Core::Render);

		int entityID = 0;
		Graphics::SpritesheetCache sheetCache;
		Graphics::Spritesheet& sheet = sheetCache.CreateFromFile("resources/pong.sheet");
		uint ballHandle = sheet.getHandle("ball");
		uint cherryHandle = sheet.getHandle("cherry");
		uint cupcakeHandle = sheet.getHandle("cupcake");
		Graphics::Texture& tex = context.resources.getTextureCache().getTexture(sheet.getTextureName().c_str());
		sheet.setTexture(tex);

		auto* ent = context.entities.emplace(std::unique_ptr<Core::Entity>(new Core::Entity(entityID++))).first->get();
		
		ent->getForm().setType(Core::FormType::Sprite);
		ent->getForm().Insert("Spritesheet", Core::IState::Create(sheet));
		ent->getForm().Insert("CurrentSprite", Core::IState::Create(cherryHandle));
		ent->getForm().setPosition(500,300);
		ent->getForm().setScale(1,-1);
		ent->getForm().setScalingCenter(sheet.getSprite(cherryHandle).getSrcRect().GetSize()/2);
		ent->getForm().setPivotPoint(sheet.getSprite(cherryHandle).getSrcRect().GetSize()/2);
		ent->getForm().setRotation(0);

		ent->Insert("SpringVel", Core::IState::Create(Util::Vec2(100,0)));
		ent->Insert("SpringMaxVel", Core::IState::Create(Util::Vec2(200,400)));
		ent->Insert("SpringAcc", Core::IState::Create(Util::Vec2(-20,40)));
		ent->Insert("SpringForce", Core::IState::Create(Util::Vec2(20,40)));

		ent->Insert("Render", Core::Action(Core::Render));
		assert(ent->getAction("Render").Activate());
		ent->Insert("Rotate", Core::Action(Core::Rotate));
		assert(ent->getAction("Rotate").Activate());
		ent->Insert("InfinityPattern", Core::Action(Core::InfinityPattern));
		assert(ent->getAction("InfinityPattern").Activate());

		ent = context.entities.emplace(std::unique_ptr<Core::Entity>(new Core::Entity(entityID++))).first->get();
		
		ent->getForm().setType(Core::FormType::Sprite);
		ent->getForm().Insert("Spritesheet", Core::IState::Create(sheet));
		ent->getForm().Insert("CurrentSprite", Core::IState::Create(cupcakeHandle));
		ent->getForm().setPosition(200,300);
		ent->getForm().setScale(2,2);
		ent->getForm().setScalingCenter(sheet.getSprite(cherryHandle).getSrcRect().GetSize()/2);
		ent->getForm().setPivotPoint(sheet.getSprite(cherryHandle).getSrcRect().GetSize()/2);
		ent->getForm().setRotation(0);

		ent->Insert("Render", Core::Action(Core::Render));
		assert(ent->getAction("Render").Activate());
		ent->Insert("Rotate", Core::Action(Core::Rotate));
		assert(ent->getAction("Rotate").Activate());

		return true;
	}
}