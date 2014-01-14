//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Pong/PongGame.h>
/******* C++ headers *******/
#include <iostream>
#include <string>
#include <vector>
/******* extra headers *******/
#include <Input/KeyCodes.h>
#include <Util/Dimensional.h>
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

	typedef uint32_t InstanceID;

	static uint32_t getLogicUpdateCount(Time& timer, const uint64_t& microsPerFrame, float& outFraction, uint64_t& outUnusedMicros)
	{
		static const uint64_t maxUpdateTime = (CORE_STEP == CORE_CLAMPED_STEP) ? CORE_MAX_MICROS_PER_FRAME : ~0ULL;

		const uint32_t maxUpdateCount = static_cast<uint32_t>(maxUpdateTime / microsPerFrame);
		const uint32_t updateCount = timer.getFixedStepUpdateCount(microsPerFrame, outFraction, outUnusedMicros);

		return updateCount < maxUpdateCount ? updateCount : maxUpdateCount;
	}

	//**************************************** PONG ****************************************//

	bool PongGame::init(Window& window)
	{
		m_timeScale = Time::NORMAL_TIME;
		m_logicTimer.update(m_timeScale);
		m_renderTimer.update(m_timeScale);

		uint32_t w = 640, h = 480;
		window.resize(w, h);

		bool initializationStatus = m_input.init(window) &&
									m_physics.init(Vec2(0, -9.0f)) &&
									m_graphics.init(window);

		if(initializationStatus)
		{
			b2BodyDef bodyDef;
			b2PolygonShape polygon;
			b2FixtureDef fixtureDef;
			b2JointDef jointDef;

			fixtureDef.density = 1;
			fixtureDef.shape = &polygon;

			//field
			bodyDef.type = b2_staticBody;
			m_field.m_body = m_physics.createBody(bodyDef);

			auto wallSize = m_physics.convert(Vec2(w*0.5f, h*0.02f));
			auto wallPos = m_physics.convert(Vec2(0, h*0.5f));
			auto goalSize = m_physics.convert(Vec2(w*0.02f, h*0.5f));
			auto goalPos = m_physics.convert(Vec2(w*0.5f, 0));

			polygon.SetAsBox(wallSize.x, wallSize.y, wallPos, 0);
			m_field.m_fixtures[0] = m_physics.createFixture(fixtureDef, m_field.m_body);
			polygon.SetAsBox(wallSize.x, wallSize.y, -wallPos, 0);
			m_field.m_fixtures[1] = m_physics.createFixture(fixtureDef, m_field.m_body);
			
			polygon.SetAsBox(goalSize.x, goalSize.y, -goalPos, 0);
			m_field.m_fixtures[2] = m_physics.createFixture(fixtureDef, m_field.m_body);
			polygon.SetAsBox(goalSize.x, goalSize.y, goalPos, 0);
			m_field.m_fixtures[3] = m_physics.createFixture(fixtureDef, m_field.m_body);

			//paddles
			bodyDef.type = b2_dynamicBody;
			bodyDef.gravityScale = 0;

			auto paddleSize = m_physics.convert(Vec2(w*0.1f, h*0.1f));
			polygon.SetAsBox(paddleSize.x, paddleSize.y);

			bodyDef.position = m_physics.convert(Vec2(w*-0.45f, 0));
			m_paddles[0].m_body = m_physics.createBody(bodyDef);
			m_paddles[0].m_fixture = m_physics.createFixture(fixtureDef, m_paddles[0].m_body);


			bodyDef.position = m_physics.convert(Vec2(w*0.45f, 0));
			m_paddles[1].m_body = m_physics.createBody(bodyDef);
			m_paddles[1].m_fixture = m_physics.createFixture(fixtureDef, m_paddles[1].m_body);
		}

		return initializationStatus;
	}

	void PongGame::shutdown()
	{
		//do some shutdown logic, like saving states and whatnot
		m_input.shutdown();
		m_physics.shutdown();
		m_graphics.shutdown();
	}

	bool PongGame::tick()
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

	bool PongGame::tickLogic(uint64_t updateTime)
	{
		bool continueRunning = true;
		m_logicTimer.updateBy(updateTime, m_timeScale);
	
		m_input.update(m_logicTimer);
		const auto& events = m_input.getEvents();

		for(auto& e : events)
		{
			switch(e.m_type)
			{
			case WE_KEYBOARDKEY:
				if(e.m_keyboard.m_keyCode == Keyboard::m_Escape)
				{
					continueRunning = false;
				}
				break;
				
			case WE_GAINFOCUS:
				m_timeScale = Time::NORMAL_TIME;
				break;
			case WE_LOSTFOCUS:
				m_timeScale = Time::STOP_TIME;
				break;
			case WE_CLOSE:
				continueRunning = false;
			default:
				break;
			}
		}

		m_graphics.setBackgroundColor(gen.randFloat(), gen.randFloat(), gen.randFloat());

		return continueRunning;
	}

	void PongGame::tickRender(uint64_t updateTime)
	{
		m_renderTimer.updateBy(updateTime, m_timeScale);
		
		//update renderer
		m_graphics.update();
	}
}
