//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "GameContextEvents_Pong.h"
	/*** C++ headers ***/
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
#include "Subsystems/Script/LuaEngine.h"
	/*** end headers ***/

namespace Pong
{
	bool ContextGameplayCreate(Core::GameContext& context)
	{
		context.actionMaster.EnqueueActionLogic(Core::PongInput);
		context.actionMaster.EnqueueActionLogic(Core::EulerMovement);
		context.actionMaster.EnqueueActionLogic(Core::RK4Movement);
		context.actionMaster.EnqueueActionLogic(Core::Collide);
		context.actionMaster.EnqueueActionLogic(Core::Rotate);
		context.actionMaster.EnqueueActionLogic(Core::Animate);
		context.actionMaster.EnqueueActionLogic(Core::Render);

		auto& sheet = context.resources.getSpritesheetCache().LoadFromFile("resources/pong.sheet");
		
		uint ballHandle = sheet.getSpriteHandle("ball");
		auto& ballSprite = sheet.getSprite(ballHandle);
		uint paddleHandle = sheet.getSpriteHandle("paddle");
		auto& paddleSprite = sheet.getSprite(paddleHandle);
		Util::Vec2 windowSize = context.services.getGraphics().getScreenSize();

		for(int i = 0; i < 2; ++i)
		{
			auto& paddle = context.entityPool.NewInstance();
			auto& paddle_form = paddle.getForm();
			paddle_form.setType(Core::FormType::Sprite);
			paddle_form.setScale(1,1);

			

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
			
			paddle_form.Insert("Spritesheet", Core::State::Create<Graphics::Spritesheet&>(sheet));
			paddle_form.Insert("CurrentSprite", Core::State::Create(paddleHandle));
			paddle_form.Insert("Velocity", Core::State::Create(Util::Vec2(0,10)));
			paddle_form.Insert("MaxVelocity", Core::State::Create(Util::Vec2(0,10)));
			paddle_form.Insert("Force", Core::State::Create(Util::Vec2(0,0)));
			paddle_form.Insert("Mass", Core::State::Create(1.0f));
			
			paddle.Insert("EulerMovement", Core::Action::Create(Core::EulerMovement));
			paddle.Insert("RK4Movement", Core::Action::Create(Core::RK4Movement));
			paddle.Insert("Render", Core::Action::Create(Core::Render));
			assert(paddle.getAction("Render")->Activate());
		}

		auto& ball = context.entityPool.NewInstance();
		auto& ball_form = ball.getForm();
		ball_form.setType(Core::FormType::Sprite);
		ball_form.setPosition(windowSize.x*0.5f, windowSize.y*0.5f);

		ball_form.Insert("Spritesheet", Core::State::Create<Graphics::Spritesheet&>(sheet));
		ball_form.Insert("CurrentSprite", Core::State::Create(ballHandle));

		ball.Insert("Render", Core::Action::Create(Core::Render));
		assert(ball.getAction("Render")->Activate());

		
		return true;
	}
}