#ifndef CORE_ACTION_FUNCTIONS_H_
#define CORE_ACTION_FUNCTIONS_H_
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
	void Render(double dt, GameContext& context, std::set<Core::Action*>& actions);
	void Animate(double dt, GameContext& context, std::set<Core::Action*>& actions);
	void Rotate(double dt, GameContext& context, std::set<Core::Action*>& actions);
	void SIH(double dt, GameContext& context, std::set<Core::Action*>& actions);
	void Collide(double dt, GameContext& context, std::set<Core::Action*>& actions);
	void EulerMovement(double dt, GameContext& context, std::set<Core::Action*>& actions);
	void RK4Movement(double dt, GameContext& context, std::set<Core::Action*>& actions);
	void PongInput(double dt, GameContext& context, std::set<Core::Action*>& actions);
}

#endif //CORE_ACTION_FUNCTIONS_H_