//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Impl/ACustom.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/
	
namespace Core
{
	void ACustom::setLogic(std::function<bool(float, GameContext&)> logic)
	{
		m_noThis = logic;
	}

	void ACustom::setLogic(std::function<bool(float)> logic)
	{
		m_withThis = logic;
	}

	bool ACustom::onUpdate(float dt)
	{
		if(m_withThis != nullptr)
		{
			return m_withThis(dt);
		}
		else if(m_noThis != nullptr)
		{
			return m_noThis(dt, m_context);
		}
		return false;
	}
}