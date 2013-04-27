#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <memory>
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "ActionLogic.h"
	/*** end header inclusion ***/

namespace Core
{
	void Render(float dt, GameContext& context, std::set<Core::Action*>& actions);
	void Animate(float dt, GameContext& context, std::set<Core::Action*>& actions);
	void Rotate(float dt, GameContext& context, std::set<Core::Action*>& actions);
	void SIH(float dt, GameContext& context, std::set<Core::Action*>& actions);
	void Collide(float dt, GameContext& context, std::set<Core::Action*>& actions);
	void EulerMovement(float dt, GameContext& context, std::set<Core::Action*>& actions);
	void RK4Movement(float dt, GameContext& context, std::set<Core::Action*>& actions);
	void PongInput(float dt, GameContext& context, std::set<Core::Action*>& actions);
}