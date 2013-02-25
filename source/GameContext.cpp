//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameContext.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Action/Action.h"
#include "Core/Action/Functions.h"
#include "Core/Entity/Entity.h"
#include "Core/Form/Form.h"
#include "Core/State/State.h"
#include "Subsystems/Graphics/Interface.h"
#include "Subsystems/Graphics/Spritesheet.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Graphics/TextureInfo.h"
	/*** end headers ***/

namespace Core
{
	GameContext::GameContext(Engine& engine, const ServiceLocator& services)
		: _engine(engine), _services(services)
	{
		_actionMaster.EnqueueAction(Core::Render);

		int entityID = 0;
		Graphics::SpritesheetCache sheetCache;
		Graphics::Spritesheet& sheet = sheetCache.CreateFromFile("resources/pong.sheet");
		uint ballHandle = sheet.getHandle("ball");
		uint cherryHandle = sheet.getHandle("cherry");
		uint cupcakeHandle = sheet.getHandle("cupcake");
		uint tex = services.getGraphics().getTextureHandle(sheet.getTextureName());
		sheet.setTextureHandle(tex);
		const Graphics::TextureInfo& texinfo = services.getGraphics().getTextureInfo(tex);


		auto* ent = _entities.emplace(std::unique_ptr<Entity>(new Entity(entityID++))).first->get();
		
		ent->getForm().setType(Core::FormType::Sprite);
		ent->getForm().setPosition(200,200);
		ent->getForm().setScale(1,1);
		ent->getForm().getStates().AddState("Spritesheet", Core::IState::Create(sheet));
		ent->getForm().getStates().AddState("CurrentSprite", Core::IState::Create(cherryHandle));
	//	ent->getForm().setPosition(500,300);
	//	ent->getForm().setScale(1,1);
		ent->getForm().setScalingCenter(sheet.getSprite(cherryHandle).getSrcRect().GetSize()/2);
		//ent->getForm().setScalingCenter(sheet.getSprite(cherryHandle).getSrcRect().GetSize());
		//ent->getForm().setScalingCenter(Util::Vec2(0,0));
		ent->getForm().setPivotPoint(sheet.getSprite(cherryHandle).getSrcRect().GetSize()/2);
		//ent->getForm().setPivotPoint(sheet.getSprite(cherryHandle).getSrcRect().GetSize());
		//ent->getForm().setPivotPoint(Util::Vec2(0,0));
		ent->getForm().setRotation(0);

		ent->getActions().Insert("Render", Action(Core::Render));
		
		auto& action = ent->getActions().Get("Render");
		action.setOwner(*ent);
	}

	void GameContext::Activate()
	{
		Action::BindActionUpdater(_actionMaster);

		auto& action = _entities.begin()->get()->getActions().Get("Render");
		assert(action.Activate());
	}

	void GameContext::Deactivate()
	{
	}

	bool GameContext::Update()
	{
		if(_timer.TimeToUpdate(gUpdateInterval))
		{
			_actionMaster.Update(_timer.getLastTimeDelta(), _services);
		}
		return true;
	}

}