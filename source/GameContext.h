#ifndef CORE_GAMECONTEXT_H_
#define CORE_GAMECONTEXT_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <memory>
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "GameContextEvent.h"
#include "ResourceLocator.h"
#include "ServiceLocator.h"
#include "Core/Action/ActionUpdater.h"
#include "Core/Entity/Entity.h"
#include "Core/Entity/EntityPool.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Graphics/TextureCache.h"
#include "Util/Clock.h"
	/*** end header inclusion ***/

namespace Core
{
	class Engine;

	class GameContext
	{
	private:
		Graphics::TextureCache _textureCache;
		Graphics::SpritesheetCache _spritesheetCache;
		
		GameContextEvent _onActivate;
		GameContextEvent _onDeactivate;
		GameContextEvent _onCreate;
		GameContextEvent _onDestroy;
		GameContextEvent _onUpdate;
	
	public:
		enum EventType
		{
			OnActivate,
			OnDeactivate,
			OnDestroy,
			OnUpdate
		};

		Util::Timer timer;
		ActionUpdater actionMaster;
		EntityPool entities;
		ServiceLocator services;
		ResourceLocator resources;

		GameContext(GameContextEvent onCreate, const ServiceLocator& services, const ResourceLocator& resources);
		~GameContext();
		void Activate();
		void Deactivate();
		bool Update();
		void setContextEventLogic(EventType type, GameContextEvent function);
	};
}

#endif //CORE_GAMECONTEXT_H_