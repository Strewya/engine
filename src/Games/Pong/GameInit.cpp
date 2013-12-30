	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Games/GameInit.h>
	/*** extra headers ***/
#include <algorithm>
#include <vector>
//#include <Games/Pong/PongGame.h>
#include <Input/KeyCodes.h>
#include <Util/Time.h>
#include <Window/Window.h>
#include <Window/WindowEvent.h>
	/*** end headers ***/

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


	typedef std::vector<WindowEvent> Events_t;

	Events_t gatherInput(uint64_t currentTimeMicros, Window& window)
	{
		Events_t evs;
		evs.reserve(20);
		WindowEvent we;
		while(window.peek(currentTimeMicros, we))
		{
			evs.emplace_back(we);
		}
		return evs;
	}

	struct PongGame
	{
		Time pongLogicTimer;
		Time pongRenderTimer;

		bool tickLogic(uint64_t updateTime, Window& window)
		{
			bool continueRunning = true;
			pongLogicTimer.updateBy(updateTime, pongLogicTimer.NORMAL_TIME);
			
			auto events = gatherInput(pongLogicTimer.getCurMicros(), window);
			
			for(auto& e : events)
			{
				switch(e.m_type)
				{
				case WindowEventType::WINDOW_CLOSE:
					//do some shutdown logic, like saving states and whatnot
					
					continueRunning = false;
					break;

				case WindowEventType::WINDOW_KEYDOWN:
					if(!e.m_keyboard.m_previouslyDown)
					{
						if(e.m_keyboard.m_keyCode == Keyboard::m_L)
							window.resize(CW_USEDEFAULT, CW_USEDEFAULT);
						else if(e.m_keyboard.m_keyCode == Keyboard::m_S)
							window.resize(640, 480);
						else if(e.m_keyboard.m_keyCode == Keyboard::m_Escape)
							window.close();
						else if(e.m_keyboard.m_keyCode == Keyboard::m_F)
							window.setFullscreen(!window.isFullscreen());
						else if(e.m_keyboard.m_keyCode == Keyboard::m_R)
							window.move(CW_USEDEFAULT, CW_USEDEFAULT);
					}
					if(e.m_keyboard.m_keyCode == Keyboard::m_ArrowUp)
						window.resize(window.getSizeX(), window.getSizeY() - 1);
					else if(e.m_keyboard.m_keyCode == Keyboard::m_ArrowDown)
						window.resize(window.getSizeX(), window.getSizeY() + 1);
					else if(e.m_keyboard.m_keyCode == Keyboard::m_ArrowLeft)
						window.resize(window.getSizeX() - 1, window.getSizeY());
					else if(e.m_keyboard.m_keyCode == Keyboard::m_ArrowRight)
						window.resize(window.getSizeX() + 1, window.getSizeY());
					break;
					

				default:
					break;
				}
			}
			
			return continueRunning;
		}

		void tickRender(uint64_t updateTime)
		{
			pongRenderTimer.updateBy(updateTime, pongRenderTimer.NORMAL_TIME);
		}

	};
	

	static uint32_t getLogicUpdateCount(Time& timer, const uint64_t microsPerFrame, float& outFraction, uint64_t& outUnusedMicros)
	{
		const uint64_t maxUpdateTime = (CORE_STEP == CORE_CLAMPED_STEP) ? CORE_MAX_MICROS_PER_FRAME : ~0ULL;

		const uint32_t maxUpdateCount = static_cast<uint32_t>(maxUpdateTime / microsPerFrame);
		const uint32_t updateCount = timer.getFixedStepUpdateCount(microsPerFrame, outFraction, outUnusedMicros);

		return std::min(updateCount, maxUpdateCount);
	}


	void initGame(Window& window)
	{
		PongGame game;
		Time m_logicTimer;
		Time m_renderTimer;

		window.resize(640, 480);

		bool running = true;
		while(running)
		{
			float fraction = 0;
			uint64_t unusedMicros = 0;
			const uint64_t microsPerFrame = CORE_MICROS_PER_FRAME;

			for(uint32_t l = getLogicUpdateCount(m_logicTimer, microsPerFrame, fraction, unusedMicros); l--;)
			{
				m_logicTimer.updateBy(microsPerFrame, m_logicTimer.NORMAL_TIME);
				if(!(running = game.tickLogic(microsPerFrame, window)))
					break;
			}

			uint64_t fullUpdateTime = m_logicTimer.getLastRealTimeMicros() + unusedMicros - m_renderTimer.getLastRealTimeMicros();
			m_renderTimer.updateBy(fullUpdateTime, m_renderTimer.NORMAL_TIME);
			game.tickRender(fullUpdateTime);
		}
	}
}


/*
WindowEvent we;
if(window.peek(timer.getCurMicros(), we))
{
	switch(we.m_type)
	{
		case WindowEventType::WINDOW_KEYDOWN:

		if(we.m_keyboard.m_keyCode == VK_ESCAPE)
		{
		quit = true;
		}

		break;

		case WindowEventType::WINDOW_MOUSEBUTTONDOWN:

		if(we.m_mouseButton.m_button == Mouse::m_LeftButton)
		{
		MessageBox(window.getWindowHandle(), "Helo", "oleh", MB_OK);
		}

		break;

		case WindowEventType::WINDOW_CLOSE:
		quit = true;
		break;

		default:
		break;
	}
}
*/