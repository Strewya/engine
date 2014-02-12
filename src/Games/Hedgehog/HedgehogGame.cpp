//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Hedgehog/HedgehogGame.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
#include <Window/Window.h>
#include <Window/WindowEvent.h>
/******* end headers *******/

namespace Core
{
#define CORE_VARIABLE_STEP 0
#define CORE_CLAMPED_STEP 1
#define CORE_FIXED_STEP 2


#define CORE_FIXED_STEP_FPS 30ULL
#define CORE_CLAMPED_STEP_MIN_FPS 15ULL

#define CORE_MICROS_PER_FRAME 1000000ULL/CORE_FIXED_STEP_FPS
#define CORE_MAX_MICROS_PER_FRAME 1000000ULL/CORE_CLAMPED_STEP_MIN_FPS


#define CORE_STEP CORE_CLAMPED_STEP

	static uint32_t getLogicUpdateCount(Time& timer, const uint64_t& microsPerFrame, float& outFraction, uint64_t& outUnusedMicros)
	{
		static const uint64_t maxUpdateTime = (CORE_STEP == CORE_CLAMPED_STEP) ? CORE_MAX_MICROS_PER_FRAME : ~0ULL;

		const uint32_t maxUpdateCount = static_cast<uint32_t>(maxUpdateTime / microsPerFrame);
		const uint32_t updateCount = timer.getFixedStepUpdateCount(microsPerFrame, outFraction, outUnusedMicros);

		return updateCount < maxUpdateCount ? updateCount : maxUpdateCount;
	}

	bool HedgehogGame::init(Window& window)
	{
		m_window = &window;

		window.resize(1024, 768);
		DEBUG_LINE(window.openConsole(780, 0));

		bool initStatus = m_scripter.init() && m_scripter.scriptFileExists(RESOURCE("Scripts/hedgehog_game.lua"));

		if(initStatus)
		{
			m_scripter.executeScriptFile(RESOURCE("Scripts/hedgehog_game.lua"));
			initStatus &= m_scripter.functionExists("game_init") && m_scripter.functionExists("game_tick") && m_scripter.functionExists("game_render");
		}
		if(initStatus)
		{
			m_scripter.executeFunction("game_init", this, CLASS(HedgehogGame));
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

	typedef std::vector<WindowEvent> EventVector_t;
	static EventVector_t gatherInput(uint64_t currentTimeMicros, Window& window)
	{
		EventVector_t evs;
		evs.reserve(20);
		WindowEvent we;
		while(window.peekEvent(currentTimeMicros, we))
		{
			evs.emplace_back(we);
		}
		return evs;
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
