#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <memory>
#include <unordered_map>
	/*** extra headers if needed (alphabetically ordered) ***/
//#include "Core/Action/Action.h"
	/*** end header inclusion ***/

namespace Core
{
	class Action;
	class GameContext;

	class ActionUpdater
	{
	public:
		ActionUpdater();
		void update(float dt, GameContext& context);
	
		bool addAction(std::unique_ptr<Action>& action);
	
	
	private:
		ActionUpdater(const ActionUpdater&);
		ActionUpdater& operator=(const ActionUpdater&);

		std::unordered_map<uint32_t, std::unique_ptr<Action>> _priorityQueue;
	};
}