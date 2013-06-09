//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameContextEvents_Pong.h"
	/*** C++ headers ***/
#include <map>
#include <memory>
	/*** extra headers ***/
#include "Engine/GameContext.h"
#include "Core/Action/Action.h"
#include "Core/Action/Functions.h"
#include "Core/Entity/Entity.h"
#include "Core/Form/Form.h"
#include "Core/State/State.h"
#include "Subsystems/Graphics/IRenderer.h"
#include "Subsystems/Graphics/Spritesheet.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Input/InputEngine.h"
#include "Subsystems/Script/LuaEngine.h"

#include "StateTest.h"
	/*** end headers ***/

namespace Pong
{
	bool CreatePaddle(Core::GameContext& context, Core::Entity& paddle)
	{
		uint hSheet = context.resources.getSpritesheetCache().getSpritesheetHandle("pong.sheet");
		auto& sheet = context.resources.getSpritesheetCache().getSpritesheet(hSheet);
		uint paddleSpriteHandle = sheet.getSpriteHandle("paddle");

		auto& form = paddle.getForm();
		form.setType(Core::FormType::Sprite);
		form.setScale(1,1);

		form.Insert("Spritesheet", Core::State::Create(hSheet));
		form.Insert("CurrentSprite", Core::State::Create(paddleSpriteHandle));
		form.Insert("Velocity", Core::State::Create(Util::Vec2(0,10)));
		form.Insert("MaxVelocity", Core::State::Create(Util::Vec2(0,10)));
		form.Insert("Force", Core::State::Create(Util::Vec2(0,0)));
		form.Insert("Mass", Core::State::Create(1.0f));

		paddle.Insert("EulerMovement", Core::Action::Create(Core::EulerMovement));
		paddle.Insert("RK4Movement", Core::Action::Create(Core::RK4Movement));
		paddle.Insert("Render", Core::Action::Create(Core::Render));
		assert(paddle.getAction("Render")->Activate());

		return true;
	}

	bool CreateBall(Core::GameContext& context, Core::Entity& ball)
	{
		uint hSheet = context.resources.getSpritesheetCache().getSpritesheetHandle("pong.sheet");
		auto& sheet = context.resources.getSpritesheetCache().getSpritesheet(hSheet);

		uint ballSpriteHandle = sheet.getSpriteHandle("ball");
		
		auto& ball_form = ball.getForm();
		ball_form.setType(Core::FormType::Sprite);
		
		ball_form.Insert("Spritesheet", Core::State::Create(hSheet));
		ball_form.Insert("CurrentSprite", Core::State::Create(ballSpriteHandle));

		ball.Insert("Render", Core::Action::Create(Core::Render));
		assert(ball.getAction("Render")->Activate());

		return false;
	}

	bool ContextGameplayCreate(Core::GameContext& context)
	{
		Core::StateTest::Position pos;
		pos.value.x = 10;
		
		
		std::map<size_t, Core::StateTest::BaseState*> test;
		test.insert(std::make_pair(pos.hash, &pos));
		

		Core::StateTest::Position* p = test[Core::StateTest::Position::hash]->downcast<Core::StateTest::Position>();
		









		context.actionMaster.EnqueueActionLogic(Core::PongInput);
		context.actionMaster.EnqueueActionLogic(Core::EulerMovement);
		context.actionMaster.EnqueueActionLogic(Core::RK4Movement);
		context.actionMaster.EnqueueActionLogic(Core::Collide);
		context.actionMaster.EnqueueActionLogic(Core::Rotate);
		context.actionMaster.EnqueueActionLogic(Core::Animate);
		context.actionMaster.EnqueueActionLogic(Core::Render);

		context.entityFactory.RegisterConstructor("paddle", CreatePaddle);
		context.entityFactory.RegisterConstructor("ball", CreateBall);

		auto windowSize = context.services.getGraphics().getScreenSize();
		uint pongSheetHandle = context.resources.getSpritesheetCache().LoadFromFile("../resources/pong.sheet");
		auto& pongSheet = context.resources.getSpritesheetCache().getSpritesheet(pongSheetHandle);

		for(int i = 0; i < 2; ++i)
		{
			auto& paddle = context.entityPool.NewInstance();

			context.entityFactory.CreateEntityType("paddle", paddle);
			auto& paddle_form = paddle.getForm();
			
			auto& paddleSprite = pongSheet.getSprite(paddle_form.getState("CurrentSprite")->as<uint>());

			if(i==0)
			{
				paddle_form.setPosition(paddleSprite.getSrcRect().GetWidth()*paddle_form.getScale().x, windowSize.y*0.5f);
				paddle_form.setColor(100, 100, 255);
				paddle.Insert("InputHandler", Core::Action::Create(Core::PongInput));
				assert(paddle.getAction("InputHandler")->Activate());
			}
			else
			{
				paddle_form.setPosition(windowSize.x - (paddleSprite.getSrcRect().GetWidth()*2)*paddle_form.getScale().x, windowSize.y*0.5f);
				paddle_form.setColor(255, 100, 100);
			}
		}

		auto& ball = context.entityPool.NewInstance();
		context.entityFactory.CreateEntityType("ball", ball);
		auto& ball_form = ball.getForm();
		ball_form.setPosition(windowSize*0.5f);

		
		return true;
	}
}