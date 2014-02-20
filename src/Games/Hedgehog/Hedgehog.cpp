//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Hedgehog/Hedgehog.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/GameLoopParams.h>
#include <Util/Utility.h>
#include <Window/Window.h>
#include <Window/WindowEvent.h>
/******* end headers *******/

namespace Core
{
	bool HedgehogGame::init(Window& window)
	{
		m_window = &window;

		window.resize(1024, 768);
		DEBUG_LINE(window.openConsole(1050, 0));

		bool initStatus = m_scripter.init() && m_scripter.scriptFileExists(RESOURCE("Scripts/hedgehog_game.lua")) && m_graphics.init(window);

		if(initStatus)
		{
			m_scripter.executeScriptFile(RESOURCE("Scripts/hedgehog_game.lua"));
			initStatus &= m_scripter.functionExists("game_init") && m_scripter.functionExists("game_tick") && m_scripter.functionExists("game_render");
		}
		if(initStatus)
		{
			m_scripter.executeFunction("game_init", this, CLASS(HedgehogGame));
//			initStatus &= m_scripting.parseScriptFile(RESOURCE("Scripts/GameInit.lua")) && m_scripting.callFunction("game_init", this, CLASS(HedgehogGame));
		}
		return initStatus;
	}

	bool HedgehogGame::tick()
	{
		float fraction = 0;
		uint64_t unusedMicros = 0;
		static const uint64_t microsPerFrame = CORE_MICROS_PER_FRAME;

		for(uint32_t l = getLogicUpdateCount(m_logicTimer, microsPerFrame, fraction, unusedMicros); l--;)
		{
			if(!tickLogic(microsPerFrame))
				return false;
		}

		uint64_t fullUpdateTime = m_logicTimer.getLastRealTimeMicros() + unusedMicros - m_renderTimer.getLastRealTimeMicros();
		tickRender(fullUpdateTime);

		return true;
	}

	bool HedgehogGame::shutdown()
	{
		m_scripter.shutdown();
		return true;
	}
	
	bool HedgehogGame::tickLogic(uint64_t updateTime)
	{
		m_scripter.executeFunction("game_tick", this, CLASS(HedgehogGame));
		return true;
	}

	void HedgehogGame::tickRender(uint64_t updateTime)
	{
		m_scripter.executeFunction("game_render", this, CLASS(HedgehogGame));
	}
}
