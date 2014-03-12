//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Pong/Pong.h>
/******* C++ headers *******/
#include <functional>
#include <iostream>
#include <string>
#include <vector>
/******* extra headers *******/
#include <Games/GameLoopParams.h>
#include <Input/KeyCodes.h>
#include <Util/Color.h>
#include <Util/Transform.h>
#include <Util/Utility.h>
#include <Util/Vec2.h>
#include <Window/Window.h>
#include <Window/WindowEvent.h>
/******* end headers *******/

namespace Core
{
	typedef uint32_t InstanceID;

	//**************************************** PONG ****************************************//

	bool PongGame::init(Window& window)
	{
		m_timeScale = Time::NORMAL_TIME;
		m_logicTimer.update(m_timeScale);
		m_renderTimer.update(m_timeScale);
		m_window = &window;
		gen.reseed(static_cast<uint32_t>(m_logicTimer.getCurMicros()));
		m_winner = 0;
		m_resetTime = 0;
		m_winScore = 10;
		m_aiActive = true;

		uint32_t screenW = 640, screenH = 480;
		float ratio = (float)screenW / (float)screenH;
		window.resize(screenW, screenH);

		DEBUG_LINE(window.openConsole(660, 0));

		bool initializationStatus = m_input.init(window) &&
			m_physics.init(Vec2(0, -9.0f), &m_debugDraw) &&
			m_scripts.init() &&
			m_graphics.init(window);
		
		if(initializationStatus)
		{
			m_debugDraw.setGraphicsSystem(m_graphics);
			m_debugDraw.AppendFlags(m_debugDraw.e_shapeBit | m_debugDraw.e_aabbBit);
			m_debugDraw.setLengthScale(1 / m_b2Scale);
			m_drawDebugData = false;

			m_physics.addBeginContactListener(std::bind(&PongGame::updateScore, this, std::placeholders::_1));
			m_physics.addEndContactListener(std::bind(&PongGame::speedUpBall, this, std::placeholders::_1));

			initializationStatus &= m_graphics.initVertexShader(RESOURCE("Shaders/shader.hlsl"));
			initializationStatus &= m_graphics.initPixelShader(RESOURCE("Shaders/shader.hlsl"));

			initializationStatus &= m_scripts.scriptFileExists(RESOURCE("Scripts/paddleAI.lua"));
			if(initializationStatus)
			{
				m_scripts.executeScriptFile(RESOURCE("Scripts/paddleAI.lua"));
			}

			auto df = m_scripts.getDataFile();
			if(df.open(RESOURCE("Sheets/font.sheet")))
			{
				initializationStatus &= m_graphics.initFont(df);
				df.close();
			}
			
			DEBUG_INFO("init status is now ", initializationStatus ? "true" : "false");

			createField();
			createBall();
			createPaddles();
		}

		return initializationStatus;
	}

	void PongGame::shutdown()
	{
		//do some shutdown logic, like saving states and whatnot
		m_input.shutdown();
		m_physics.shutdown();
		m_scripts.shutdown();
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
				else if(m_resetTime == 0 && e.m_keyboard.m_keyCode == Keyboard::m_Enter && e.m_keyboard.m_isDown && !e.m_keyboard.m_previouslyDown)
				{
					auto* body = m_physics.getBody(m_ball.m_body);
					if(body)
					{
						float f = gen.randFloat();
						float sx = gen.randFloat();
						float sy = gen.randFloat();
						float x = f > 0.5f ? f : 1 - f;
						float y = 1 - x;
						x = sx < 0.5f ? x : -x;
						y = sy < 0.5f ? y : -y;
						body->ApplyLinearImpulse(b2Vec2(x * 20, y * 10), body->GetWorldCenter());
					}
				}
				else if(e.m_keyboard.m_keyCode == Keyboard::m_W)
				{
					if(e.m_keyboard.m_isDown && !e.m_keyboard.m_previouslyDown)
					{
						m_leftPaddle.m_targetVelocity += m_leftPaddle.m_maxVelocity;
					}
					if(!e.m_keyboard.m_isDown)
					{
						m_leftPaddle.m_targetVelocity -= m_leftPaddle.m_maxVelocity;
					}
				}
				else if(e.m_keyboard.m_keyCode == Keyboard::m_S)
				{
					if(e.m_keyboard.m_isDown && !e.m_keyboard.m_previouslyDown)
					{
						m_leftPaddle.m_targetVelocity -= m_leftPaddle.m_maxVelocity;
					}
					if(!e.m_keyboard.m_isDown)
					{
						m_leftPaddle.m_targetVelocity += m_leftPaddle.m_maxVelocity;
					}
				}
				else if(!m_aiActive && e.m_keyboard.m_keyCode == Keyboard::m_ArrowUp)
				{
					if(e.m_keyboard.m_isDown && !e.m_keyboard.m_previouslyDown)
					{
						m_rightPaddle.m_targetVelocity += m_rightPaddle.m_maxVelocity;
					}
					if(!e.m_keyboard.m_isDown)
					{
						m_rightPaddle.m_targetVelocity -= m_rightPaddle.m_maxVelocity;
					}
				}
				else if(!m_aiActive && e.m_keyboard.m_keyCode == Keyboard::m_ArrowDown)
				{
					if(e.m_keyboard.m_isDown && !e.m_keyboard.m_previouslyDown)
					{
						m_rightPaddle.m_targetVelocity -= m_rightPaddle.m_maxVelocity;
					}
					if(!e.m_keyboard.m_isDown)
					{
						m_rightPaddle.m_targetVelocity += m_rightPaddle.m_maxVelocity;
					}
				}
#ifdef _DEBUG
				else if(e.m_keyboard.m_keyCode == Keyboard::m_F2 && e.m_keyboard.m_isDown && !e.m_keyboard.m_previouslyDown)
				{
					m_drawDebugData = !m_drawDebugData;
				}
				else if(e.m_keyboard.m_keyCode == Keyboard::m_Space && e.m_keyboard.m_isDown && !e.m_keyboard.m_previouslyDown)
				{
					m_ball.m_reset = true;
				}
#endif
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

		if(m_aiActive)
		{
			updatePaddleAI();
		}

		auto* body = m_physics.getBody(m_leftPaddle.m_body);
		auto vel = body->GetLinearVelocity();
		auto velChange = m_b2Scale*m_leftPaddle.m_targetVelocity - vel.y;
		auto impulse = body->GetMass() * velChange;
		body->ApplyLinearImpulse(b2Vec2(0, impulse), body->GetWorldCenter());

		body = m_physics.getBody(m_rightPaddle.m_body);
		vel = body->GetLinearVelocity();
		velChange = m_b2Scale*m_rightPaddle.m_targetVelocity - vel.y;
		impulse = body->GetMass() * velChange;
		body->ApplyLinearImpulse(b2Vec2(0, impulse), body->GetWorldCenter());

		m_physics.update(m_logicTimer.getDeltaTime());

		body = m_physics.getBody(m_ball.m_body);
		if(body)
		{
			m_ball.m_tf.position = convert(body->GetPosition())*(1 / m_b2Scale);
		}

		body = m_physics.getBody(m_leftPaddle.m_body);
		if(body)
		{
			m_leftPaddle.m_tf.position = convert(body->GetPosition())*(1 / m_b2Scale);
		}

		body = m_physics.getBody(m_rightPaddle.m_body);
		if(body)
		{
			m_rightPaddle.m_tf.position = convert(body->GetPosition())*(1 / m_b2Scale);
		}
		body = m_physics.getBody(m_ball.m_body);
		if(body)
		{
			if(m_ball.m_reset)
			{
				body->SetTransform(b2Vec2(0, 0), 0);
				body->SetLinearVelocity(b2Vec2(0, 0));
				m_ball.m_reset = false;
			}
			if(m_ball.m_speedup)
			{
				vel = body->GetLinearVelocity();
				DEBUG_INFO("pre-impulse: vel:", vel.x, ",", vel.y);
				
				b2Vec2 impulse = vel;
				impulse.Normalize();
				impulse = body->GetMass() * impulse;

				auto length = impulse.Normalize();
				impulse += convert(m_ball.m_sway);
				impulse *= length;
				
				if((vel + impulse).LengthSquared() < m_ball.m_maxVelocity*m_ball.m_maxVelocity*m_b2Scale)
				{
					body->ApplyLinearImpulse(impulse, body->GetWorldCenter());
				}
				vel = body->GetLinearVelocity();
				DEBUG_INFO("post-impulse: vel:", vel.x, ",", vel.y);
				m_ball.m_speedup = false;
				m_ball.m_sway.set(0, 0);
			}
			m_ball.m_velocity = convert(body->GetLinearVelocity()) / m_b2Scale;
		}

		if(m_leftPaddle.m_score == m_winScore)
		{
			m_winner = 1;
			m_resetTime = m_logicTimer.getCurMicros() + Time::secondsToMicros(5);
			m_leftPaddle.m_score = 0;
			m_rightPaddle.m_score = 0;
		}
		if(m_rightPaddle.m_score == m_winScore)
		{
			m_winner = 2;
			m_resetTime = m_logicTimer.getCurMicros() + Time::secondsToMicros(5);
			m_leftPaddle.m_score = 0;
			m_rightPaddle.m_score = 0;
		}
		
		if(m_resetTime != 0 && m_resetTime <= m_logicTimer.getCurMicros())
		{
			m_winner = 0;
			m_resetTime = 0;
			m_ball.m_reset = true;
		}

		return continueRunning;
	}

	void PongGame::tickRender(uint64_t updateTime)
	{
		m_renderTimer.updateBy(updateTime, m_timeScale);
		
		//update renderer
		//m_graphics.update();

		auto h = m_window->getSizeY()*0.5f - 20;

		m_graphics.begin();

		

		m_graphics.drawQuad(m_field.m_tf, m_field.m_size*0.5f, m_field.m_c);

		if(m_resetTime == 0)
		{
			m_graphics.drawQuad(m_ball.m_tf, m_ball.m_size*0.5f, m_ball.m_c);
		}

		Transform textTransform;
		textTransform.rotation = 0;
		textTransform.scale.set(1, 1);
		
		m_graphics.drawQuad(m_leftPaddle.m_tf, m_leftPaddle.m_size*0.5f, m_leftPaddle.m_c);
		textTransform.position.set(m_leftPaddle.m_tf.position.x, h);
		m_graphics.drawText(std::to_string(m_leftPaddle.m_score), textTransform, m_leftPaddle.m_c, 0);

		m_graphics.drawQuad(m_rightPaddle.m_tf, m_rightPaddle.m_size*0.5f, m_rightPaddle.m_c);
		textTransform.position.set(m_rightPaddle.m_tf.position.x, h);
		m_graphics.drawText(std::to_string(m_rightPaddle.m_score), textTransform, m_rightPaddle.m_c, 2);

		static const std::string players[2] = {"Red", "Blue"};
		if(m_winner != 0)
		{
			textTransform.position.set(0, 0);
			m_graphics.drawText(players[m_winner - 1] + " player WINS!", textTransform, Color(1, 1, 1), 1);
		}
		else
		{
			textTransform.position.set(0, h);
			textTransform.scale.set(0.5f, 0.5f);
			m_graphics.drawText("Press Enter to launch ball, Escape to exit", textTransform, Color(1, 1, 1), 1);
		}
		
		if(m_drawDebugData)
			m_physics.draw();

		m_graphics.present();
	}

	void PongGame::updateScore(b2Contact* contact)
	{
		auto fA = contact->GetFixtureA();
		auto fB = contact->GetFixtureB();
		DEBUG_INFO("Update score called");

		if((fA->IsSensor() && fB->GetBody()->GetUserData() == &m_ball) ||
		   (fB->IsSensor() && fA->GetBody()->GetUserData() == &m_ball))
		{
			DEBUG_INFO("Increasing score");
			auto sensor = fA->IsSensor() ? fA : fB;
			auto paddle = (Paddle*)sensor->GetUserData();
			++paddle->m_score;

			m_ball.m_reset = true;
		}
		
	}

	void PongGame::speedUpBall(b2Contact* contact)
	{
		auto fA = contact->GetFixtureA();
		auto fB = contact->GetFixtureB();
		
		auto bA = fA->GetBody();
		auto bB = fB->GetBody();

		b2Body* paddle = nullptr;
		b2Body* ball = nullptr;

		if(bA->GetUserData() == &m_ball)
		{
			ball = bA;
			paddle = bB;
		}
		else if(bB->GetUserData() == &m_ball)
		{
			ball = bB;
			paddle = bA;
		}
		else
		{
			DEBUG_INFO("No ball in collision, leaving");
			return;
		}
		
		if(paddle->GetUserData() != &m_leftPaddle && paddle->GetUserData() != &m_rightPaddle)
		{
			DEBUG_INFO("No paddle in collision, leaving");
			return;
		}
		
		m_ball.m_speedup = true;
		auto sway = ball->GetPosition() - paddle->GetPosition();
		sway.Normalize();
		DEBUG_INFO("Sway will be ", sway.x, ",", sway.y);
		m_ball.m_sway = convert(sway)*2;
	}

	void PongGame::updatePaddleAI()
	{
		m_scripts.executeFunction("paddleAI", this, CLASS(PongGame));
	}

	void PongGame::createField()
	{
		b2BodyDef bodyDef;
		b2FixtureDef fixtureDef;
		b2PolygonShape polygonShape;

		bodyDef.type = b2_staticBody;
		fixtureDef.density = 1;
		fixtureDef.shape = &polygonShape;

		m_field.m_tf.position.set(0, 0);
		m_field.m_tf.scale.set(1, 1);
		m_field.m_tf.rotation = 0;
		m_field.m_c.set(0.1f, 0.1f, 0.1f);
		m_field.m_size.set(600, 400);

		
		bodyDef.userData = (void*)&m_field;
		bodyDef.position.Set(0, 0);
		m_field.m_body = m_physics.createBody(bodyDef);

		
		uint32_t i = 0;
		//top and bottom walls
		polygonShape.SetAsBox(m_field.m_size.x*m_b2Scale*0.7f, 3, convert(Vec2(0, m_field.m_size.y*0.5f)*m_b2Scale + Vec2(0, 3)), 0);
		m_field.m_fixtures[i++] = m_physics.createFixture(fixtureDef, m_field.m_body);

		polygonShape.SetAsBox(m_field.m_size.x*m_b2Scale*0.7f, 3, convert(Vec2(0, -m_field.m_size.y*0.5f)*m_b2Scale + Vec2(0, -3)), 0);
		m_field.m_fixtures[i++] = m_physics.createFixture(fixtureDef, m_field.m_body);

		//left and right goal
		fixtureDef.isSensor = true;

		fixtureDef.userData = (void*)&m_rightPaddle;
		polygonShape.SetAsBox(3, m_field.m_size.y*m_b2Scale*0.7f, convert(Vec2(-m_field.m_size.x*0.5f, 0)*m_b2Scale + Vec2(-3, 0)), 0);
		m_field.m_fixtures[i++] = m_physics.createFixture(fixtureDef, m_field.m_body);

		fixtureDef.userData = (void*)&m_leftPaddle;
		polygonShape.SetAsBox(3, m_field.m_size.y*m_b2Scale*0.7f, convert(Vec2(m_field.m_size.x*0.5f, 0)*m_b2Scale + Vec2(3, 0)), 0);
		m_field.m_fixtures[i++] = m_physics.createFixture(fixtureDef, m_field.m_body);
	}

	void PongGame::createPaddles()
	{
		b2BodyDef bodyDef;
		b2PolygonShape polygonShape;
		b2FixtureDef fixtureDef;
		b2PrismaticJointDef jointDef;

		auto* bodyB = m_physics.getBody(m_field.m_body);

		bodyDef.type = b2_dynamicBody;
		bodyDef.gravityScale = 0;
		fixtureDef.density = 1;
		fixtureDef.shape = &polygonShape;
		jointDef.collideConnected = true;

		//left paddle
		m_leftPaddle.m_tf.position.set(-290, 0);
		m_leftPaddle.m_tf.scale.set(1, 1);
		m_leftPaddle.m_tf.rotation = 0;
		m_leftPaddle.m_c.set(1, 0, 0);
		m_leftPaddle.m_size.set(15, 80);
		m_leftPaddle.m_maxVelocity = 100;
		m_leftPaddle.m_targetVelocity = 0;
		m_leftPaddle.m_score = 0;

		bodyDef.userData = (void*)&m_leftPaddle;
		bodyDef.position = convert(m_leftPaddle.m_tf.position*m_b2Scale);
		m_leftPaddle.m_body = m_physics.createBody(bodyDef);

		polygonShape.SetAsBox(m_leftPaddle.m_size.x*m_b2Scale * 2, m_leftPaddle.m_size.y*m_b2Scale*0.55f, b2Vec2(-m_leftPaddle.m_size.x*m_b2Scale*1.5f, 0), 0);
		m_leftPaddle.m_fixture = m_physics.createFixture(fixtureDef, m_leftPaddle.m_body);

		auto* bodyA = m_physics.getBody(m_leftPaddle.m_body);
		jointDef.Initialize(bodyA, bodyB, bodyB->GetWorldCenter(), b2Vec2(0, 1));
		m_leftPaddle.m_joint = m_physics.createJoint(jointDef);



		//right paddle
		m_rightPaddle.m_tf.position.set(290, 0);
		m_rightPaddle.m_tf.scale.set(1, 1);
		m_rightPaddle.m_tf.rotation = 0;
		m_rightPaddle.m_c.set(0, 0, 1);
		m_rightPaddle.m_size.set(15, 80);
		m_rightPaddle.m_maxVelocity = 100;
		m_rightPaddle.m_targetVelocity = 0;
		m_rightPaddle.m_score = 0;

		bodyDef.userData = (void*)&m_rightPaddle;
		bodyDef.position = convert(m_rightPaddle.m_tf.position*m_b2Scale);
		m_rightPaddle.m_body = m_physics.createBody(bodyDef);
		
		polygonShape.SetAsBox(m_rightPaddle.m_size.x*m_b2Scale * 2, m_rightPaddle.m_size.y*m_b2Scale*0.55f, b2Vec2(m_rightPaddle.m_size.x*m_b2Scale*1.5f, 0), 0);
		m_rightPaddle.m_fixture = m_physics.createFixture(fixtureDef, m_rightPaddle.m_body);

		bodyA = m_physics.getBody(m_rightPaddle.m_body);
		jointDef.Initialize(bodyA, bodyB, bodyB->GetWorldCenter(), b2Vec2(0, 1));
		m_rightPaddle.m_joint = m_physics.createJoint(jointDef);
	}

	void PongGame::createBall()
	{
		b2BodyDef bodyDef;
		b2FixtureDef fixtureDef;
		b2PolygonShape polygonShape;

		bodyDef.type = b2_dynamicBody;
		bodyDef.gravityScale = 0;
		bodyDef.fixedRotation = true;
		fixtureDef.restitution = 1;
		fixtureDef.friction = 0;
		fixtureDef.density = 1;
		fixtureDef.shape = &polygonShape;

		m_ball.m_tf.position.set(0, 0);
		m_ball.m_tf.scale.set(1, 1);
		m_ball.m_tf.rotation = 0;
		m_ball.m_c.set(1, 1, 1);
		m_ball.m_size.set(10, 10);
		m_ball.m_reset = false;
		m_ball.m_speedup = false;
		m_ball.m_maxVelocity = 500;
		m_ball.m_sway.set(0, 0);

		
		bodyDef.userData = (void*)&m_ball;
		m_ball.m_body = m_physics.createBody(bodyDef);

		polygonShape.SetAsBox(m_ball.m_size.x*m_b2Scale*0.5f, m_ball.m_size.y*m_b2Scale*0.5f);
		m_ball.m_fixture = m_physics.createFixture(fixtureDef, m_ball.m_body);
	}
}
