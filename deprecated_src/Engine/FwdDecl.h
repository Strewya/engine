#pragma once

#include <memory>

namespace Core
{
	
	class Engine;
	
	class EntityFactory;
	class EntityPool;
	class GameContext;
	class ServiceLocator;
	class ResourceLocator;


	class Action;
	typedef Action& ActionRef;
	typedef Action* ActionRptr;
	typedef std::unique_ptr<Action> ActionUptr;
	class Entity;
	class State;
	typedef State& StateRef;
	typedef State* StateRptr;
	typedef std::unique_ptr<State> StateUptr;
}