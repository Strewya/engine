#pragma once
/********************************************
*	class:	PongGame
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Graphics/GraphicsSystem.h>
#include <Input/InputSystem.h>
#include <Physics/PhysicsSystem.h>
#include <Util/Random.h>
#include <Util/Time.h>
/******* end header inclusion *******/

namespace Core
{
	class Window;

	struct Paddle
	{
		InstanceID m_body;
		InstanceID m_fixture;
		InstanceID m_joint;
		uint32_t m_maxVelocity;
	};

	struct Field
	{
		InstanceID m_body;
		InstanceID m_fixtures[4];
	};

	struct Ball
	{
		InstanceID m_body;
		InstanceID m_fixture;
	};

	struct PongGame
	{
		Time m_logicTimer;
		Time m_renderTimer;
		double m_timeScale;
		Random gen;

		InputSystem m_input;
		PhysicsSystem m_physics;
		GraphicsSystem m_graphics;

		Paddle m_paddles[2];
		Field m_field;
		Ball m_ball;

		bool init(Window& window);
		bool tick();
		void shutdown();

		bool tickLogic(uint64_t updateTime);
		void tickRender(uint64_t updateTime);


	};
}
