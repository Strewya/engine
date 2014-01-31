#pragma once
/********************************************
*	class:	PongGame
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/Box2dDebugDraw.h>
#include <Graphics/GraphicsSystem.h>
#include <Input/InputSystem.h>
#include <Physics/PhysicsSystem.h>
#include <Scripting/ScriptingSystem.h>
#include <Util/Color.h>
#include <Util/Random.h>
#include <Util/Time.h>
#include <Util/Transform.h>
/******* end header inclusion *******/

namespace Core
{
	class Window;

	struct Paddle
	{
		//logic
		uint32_t m_score;
		//physics
		InstanceID m_body;
		InstanceID m_fixture;
		InstanceID m_joint;
		float m_maxVelocity;
		float m_targetVelocity;
		//render
		Vec2 m_size;
		Color m_c;
		Transform m_tf;
	};

	struct Field
	{
		//physics
		InstanceID m_body;
		InstanceID m_fixtures[4];
		//render
		Vec2 m_size;
		Color m_c;
		Transform m_tf;
	};

	struct Ball
	{
		//logic
		bool m_reset;
		bool m_speedup;
		float m_maxVelocity;
		Vec2 m_sway;
		//physics
		InstanceID m_body;
		InstanceID m_fixture;
		
		//rendering
		Vec2 m_size;
		Color m_c;
		Transform m_tf;		
	};

	struct PongGame
	{
		Time m_logicTimer;
		Time m_renderTimer;
		double m_timeScale;
		Random gen;
		const float m_b2Scale = 1.0f / 10.0f;
		bool m_drawDebugData;
		Window* m_window;

		InputSystem m_input;
		PhysicsSystem m_physics;
		ScriptingSystem m_scripts;
		GraphicsSystem m_graphics;
		Box2dDebugDraw m_debugDraw;

		Paddle m_leftPaddle, m_rightPaddle;
		Field m_field;
		Ball m_ball;
		uint32_t m_winScore;
		uint32_t m_winner;
		uint64_t m_resetTime;

		bool init(Window& window);
		bool tick();
		void shutdown();

		bool tickLogic(uint64_t updateTime);
		void tickRender(uint64_t updateTime);

		void createField();
		void createPaddles();
		void createBall();
		void updateScore(b2Contact* contact);
		void speedUpBall(b2Contact* contact);
	};
}
