//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Core/Action/Impl/Render.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Core/Entity/Entity.h"
#include "Engine/GameContext.h"
#include "Engine/ServiceLocator.h"
#include "Services/Graphics/IRenderer.h"
#include "Services/Graphics/Polygon.h"
	/*** end headers ***/

namespace Core
{
	bool ARender::onUpdate(float dt, GameContext& context)
	{
		for(auto id : _entities)
		{
			//check if entity still exists, if not add it to deletion list
			if(context.entityPool.isAlive(id))
			{
			//[exists]
			//get entity
				//Entity* ent = context.entityPool.getInstancePtr(id);
			//get required components:

			//	RenderData
			//	Position
			//	..
			//check if entity should be drawn

			//[is drawn]
			//add entity to _queue
			}
		}
		//remove entities that are no longer valid from the list

		context.services.getGraphics().BeginScene();
		
//		_queue.Render(context.services, context.resources);
		context.physicsWorld.DrawDebugData();

		context.services.getGraphics().EndScene();

		return true;
	}



	/*
	void Render(float dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		Graphics::RenderingQueue alphaQueue;
		for(auto action : actions)
		{
			alphaQueue.Add(action->getOwner().getForm());
		}

		context.services.getGraphics().BeginScene();
		context.services.getGraphics().BeginSpriteBatch(true);
		
		alphaQueue.Render(context.services, context.resources);

		context.services.getGraphics().EndSpriteBatch();
		context.services.getGraphics().EndScene();
	}

	void Animate(float dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		StateRptr s_currentSprite = nullptr;
		StateRptr s_animationFrame = nullptr;
		StateRptr s_animation = nullptr;
		StateRptr s_spritesheet = nullptr;
		StateRptr s_timer = nullptr;

		for(auto action : actions)
		{
			auto& owner = action->getOwner().getForm();
			s_spritesheet = owner.getState("Spritesheet");
			s_animation = owner.getState("CurrentAnimation");
			s_animationFrame = owner.getState("CurrentAnimationFrame");
			s_currentSprite = owner.getState("CurrentSprite");
			s_timer = owner.getState("AnimationTimer");
			if(s_currentSprite && s_animation && s_animationFrame && s_spritesheet)
			{
				if(s_timer && !s_timer->as<Util::Timer>().HasUpdatePeriodElapsed())
				{
					continue;
				}
				uint32_t hSheet = s_spritesheet->as<uint32_t>();
				auto* spritesheet = context.resources.getSpritesheetCache().getSpritesheet(hSheet);
				auto& animation = spritesheet->getAnimation(s_animation->as<uint32_t>());
				auto& curFrame = s_animationFrame->as<uint32_t>();
				auto& curSprite = s_currentSprite->as<uint32_t>();
				curFrame = (++curFrame) % animation.getSequenceSize();
				curSprite = animation.getSequenceFrame(curFrame);
			}
		}
	}

	void Rotate(float dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		for(auto action : actions)
		{
			action->getOwner().getForm().Rotate(1);
		}
	}

	void PongInput(float dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		if(context.services.getInput().isPressed(Input::Keyboard::_Escape))
		{
			context.services.getEngine().Shutdown();
		}

		for(auto action : actions)
		{
			auto& entity = action->getOwner();
			auto& form = entity.getForm();
			Util::Vec2& vel = form.getState("Velocity")->as<Util::Vec2>();
			auto a_euler = entity.getAction("EulerMovement");
			auto a_rk4 = entity.getAction("RK4Movement");
			if(!a_euler || !a_rk4) continue;
			if(context.services.getInput().isPressed(Input::Keyboard::_ArrowDown))
			{
				if(vel.x < 0) vel.x = -vel.x;
				if(vel.y < 0) vel.y = -vel.y;

				a_euler->Activate();
			}
			else if(context.services.getInput().isPressed(Input::Keyboard::_ArrowUp))
			{
				if(vel.x > 0) vel.x = -vel.x;
				if(vel.y > 0) vel.y = -vel.y;
				a_euler->Activate();
			}
			else
			{
				a_euler->Deactivate();
			}

			if(context.services.getInput().isPressed(Input::Keyboard::_S))
			{
				if(vel.x < 0) vel.x = -vel.x;
				if(vel.y < 0) vel.y = -vel.y;

				a_rk4->Activate();
			}
			else if(context.services.getInput().isPressed(Input::Keyboard::_W))
			{
				if(vel.x > 0) vel.x = -vel.x;
				if(vel.y > 0) vel.y = -vel.y;
				a_rk4->Activate();
			}
			else
			{
				a_rk4->Deactivate();
			}
		}
	}

	void EulerMovement(float dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		for(auto action : actions)
		{
			auto& form = action->getOwner().getForm();
			StateRptr s_velocity = form.getState("Velocity");
			StateRptr s_maxvel = form.getState("MaxVelocity");
			StateRptr s_force = form.getState("Force");
			StateRptr s_mass = form.getState("Mass");
			if(s_velocity && s_force && s_mass)
			{
				Util::Vec2& velocity = s_velocity->as<Util::Vec2>();
				Util::Vec2& maxvel = s_maxvel->as<Util::Vec2>();
				Util::Vec2& force = s_force->as<Util::Vec2>();
				float& mass = s_mass->as<float>();
				Util::Vec2 position = form.getPosition();
				position += velocity*dt;
				velocity += dt*force/mass;
				velocity.Truncate(maxvel.Length());
				form.Translate(velocity);
			}
		}
	}

	struct physicsState
	{
		Util::Vec2 pos;
		Util::Vec2 vel;
		Util::Vec2 force;
	};

	struct physicsDerivative
	{
		Util::Vec2 vel;
		Util::Vec2 acc;
	};

	Util::Vec2 accelerate(const physicsState& state)
	{
		return state.force;
	}

	physicsDerivative evaluate(const physicsState& initial, float dt, const physicsDerivative& d)
	{
		physicsState state;
        state.pos = initial.pos + d.vel*dt;
        state.vel = initial.vel + d.acc*dt;
		state.force = initial.force;

        physicsDerivative output;
        output.vel = state.vel;
        output.acc = accelerate(state);
        return output;
	}

	void integrate(physicsState& state, float dt)
	{
		physicsDerivative a = evaluate(state, 0.0f, physicsDerivative());
        physicsDerivative b = evaluate(state, dt*0.5f, a);
        physicsDerivative c = evaluate(state, dt*0.5f, b);
        physicsDerivative d = evaluate(state, dt, c);

        const Util::Vec2 dxdt = 1.0f/6.0f * (a.vel + 2.0f*(b.vel + c.vel) + d.vel);
        const Util::Vec2 dvdt = 1.0f/6.0f * (a.acc + 2.0f*(b.acc + c.acc) + d.acc);

        state.pos += dxdt;
        state.vel += dvdt;
	}

	void RK4Movement(float dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		for(auto action : actions)
		{
			auto& form = action->getOwner().getForm();
			StateRptr s_velocity = form.getState("Velocity");
			StateRptr s_force = form.getState("Force");
			StateRptr s_mass = form.getState("Mass");
			if(s_velocity && s_force && s_mass)
			{
				Util::Vec2& vel = s_velocity->as<Util::Vec2>();
				physicsState state = {form.getPosition(), vel, s_force->as<Util::Vec2>()};
				integrate(state, dt);
				vel = state.vel;
				form.setPosition(state.pos);
			}
		}
	}

	void Collide(float dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		Space* space;
		InstanceID spaceID = context.spacePool.getNewInstance(&space);
		for(auto action : actions)
		{
			space->AddEntity(action->getOwner().getID());
		}

		auto entityID = space->begin();
		for(; entityID != space->end(); ++entityID)
		{
			for(auto otherID = entityID; otherID != space->end(); ++otherID)
			{
				if(*entityID != *otherID)
				{
					auto& entity = context.entityPool.Retrieve(*entityID);
					auto& other = context.entityPool.Retrieve(*otherID);
					StateRptr s_entityRect = entity.getForm().getState("CollisionRect");
					StateRptr s_otherRect = other.getForm().getState("CollisionRect");
					if(s_entityRect != nullptr && s_otherRect != nullptr)
					{
						Util::Rect& entityRect = s_entityRect->as<Util::Rect>();
						Util::Rect& otherRect = s_otherRect->as<Util::Rect>();
						if(entityRect.IsIntersected(otherRect))
						{
							auto a_entityResponse = entity.getAction("CollisionResponse");
							auto a_otherResponse = other.getAction("CollisionResponse");
							if(a_entityResponse != nullptr)
							{
								a_entityResponse->Activate();
							}
							if(a_otherResponse != nullptr)
							{
								a_otherResponse->Activate();
							}
						}
					}
				}
			}
		}
	}

	void SIH(float dt, GameContext& context, std::set<Core::Action*>& actions)
	{
		float speed = 5;
		for(auto action : actions)
		{
			auto& entity = action->getOwner();
			if(context.services.getInput().isPressed(Input::Keyboard::_ArrowLeft))
			{
				entity.getForm().Translate(-speed, 0);
			}
			if(context.services.getInput().isPressed(Input::Keyboard::_ArrowRight))
			{
				entity.getForm().Translate(speed, 0);
			}
			if(context.services.getInput().isPressed(Input::Keyboard::_ArrowDown))
			{
				entity.getForm().Translate(0, speed);
			}
			if(context.services.getInput().isPressed(Input::Keyboard::_ArrowUp))
			{
				entity.getForm().Translate(0, -speed);
			}
		}

		Input::Event event;
		while(context.services.getInput().PollEvent(event))
		{
			for(auto action : actions)
			{
				switch(event.type)
				{
				case Input::EventType::KeyPressed:
					{
						if(event.key.code == Input::Keyboard::_Space)
						{
							action->getOwner().getForm().setVisibility(!action->getOwner().getForm().getVisibility());
						}
					}
				}
			}
		}

		if(context.services.getInput().isPressed(Input::Keyboard::_Escape))
		{
			context.services.getEngine().Shutdown();
		}
	}
	*/
}
