#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <functional>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/Action.h"
	/*** end header inclusion ***/
	
namespace Core
{
	SYSTEM(ACustom)
	{
	public:
		SYSTEM_CTOR(ACustom);
		void setLogic(std::function<bool(float)> logic);
		void setLogic(std::function<bool(float, Action&)> logic);

	protected:
		bool onUpdate(float dt);

	private:
		std::function<bool(float, GameContext&)> m_noThis;
		std::function<bool(float)> m_withThis;
	};
}