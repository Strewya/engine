//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/GameObjects.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Random.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	void movePlayers(const Time& timer, VPlayers& players, uint32_t activePlayerCount, const Rect& playingField)
	{
		for(uint32_t i = 0; i < activePlayerCount; ++i)
		{
			Player& player = players[i];
			player.velocity += (player.acceleration*Vec2::normalize(player.direction));
			Vec2 dir;
			dir.x = std::fabs(player.direction.x);
			dir.y = std::fabs(player.direction.y);
			player.velocity *= dir;
			clamp(-player.maxVelocity.x, player.maxVelocity.x, player.velocity.x);
			clamp(-player.maxVelocity.y, player.maxVelocity.y, player.velocity.y);
			player.transform.position += (timer.getDeltaTime()*player.velocity);
			Rect playerBBox = player.boundingBox;
			playerBBox.center = player.transform.position;
			if(!isRectInsideRect(playerBBox, playingField))
			{
				clamp(playingField.left() + player.boundingBox.halfWidth, playingField.right() - player.boundingBox.halfWidth, player.transform.position.x);
				clamp(playingField.bottom() + player.boundingBox.halfHeight, playingField.top() - player.boundingBox.halfHeight, player.transform.position.y);
			}
		}
	}

	void fireWeapon(Player& player, VRayBullets& bullets)
	{
		switch(player.currentWeapon)
		{
			case Pistol:
				if(player.weaponTimer.getCurMicros() > Time::secondsToMicros(0.5f))
				{
					player.weaponTimer.reset();
					generateBullets(bullets, 1, 0, player.transform.position, player.aim);
				}
				break;
			case Shotgun:
				if(player.weaponTimer.getCurMicros() > Time::secondsToMicros(1))
				{
					player.weaponTimer.reset();
					generateBullets(bullets, 6, 2.0f, player.transform.position, player.aim);
				}
				break;
			case Uzi:
				if(player.weaponTimer.getCurMicros() > Time::secondsToMicros(0.1f))
				{
					player.weaponTimer.reset();
					generateBullets(bullets, 1, 1.0f, player.transform.position, player.aim);
				}
				break;
		}
	}

	void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target)
	{
		if(monsters.size() > 150) return;

		monsters.emplace_back();
		Random gen{Time::microsToMilis(Time::getRealTimeMicros())};
		auto& monster = monsters.back();
		monster.boundingBox.set(0, 0, 1, 1);
		monster.color.set(0, 0, 0);
		monster.maxVelocity = 1 + gen.randFloat() * 2;
		monster.transform.position = position;
		monster.targetPlayer = target;
	}

	void moveMonsters(const Time& timer, VMonsters& monsters, const VPlayers& players)
	{
		for(auto& monster : monsters)
		{
			monster.direction = Vec2::normalize(players[monster.targetPlayer].transform.position - monster.transform.position);
			monster.transform.position += (monster.direction*monster.maxVelocity*timer.getDeltaTime());
		}
	}

	void updateMonsterSpawners(const Time& timer, VMonsterSpawners& spawners, VMonsters& monsters, uint32_t playerCount)
	{
		Random gen{Time::microsToMilis(Time::getRealTimeMicros())};
		for(auto& spawner : spawners)
		{
			spawner.timer.updateBy(timer.getDeltaMicros());
			if(spawner.timer.getCurMicros() > spawner.spawnCooldown)
			{
				spawner.timer.reset();
				generateMonster(monsters, spawner.transform.position + Vec2{(gen.randFloat() * 2 - 1)*spawner.spawnRadius, (gen.randFloat() * 2 - 1)*spawner.spawnRadius}, gen.randInt(0,playerCount-1));
			}
		}
	}

	void generateBullets(VRayBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target)
	{

		Vec2 direction = Vec2::normalize(target - origin);
		Vec2 targetDistance = direction * 15;
		Random gen(Time::countMilisInMicros(Time::getRealTimeMicros()));
		for(uint32_t i = 0; i < count; ++i)
		{
			Vec2 t{gen.randFloat()*spread * 2 - spread, gen.randFloat()*spread * 2 - spread};
			bullets.emplace_back(RayBullet{origin, origin, Vec2::normalize(targetDistance + t)*30, 0});
		}
	}

	void moveBullets(const Time& timer, VRayBullets& bullets)
	{
		for(uint32_t i = 0; i < bullets.size();) 
		{
			auto displacement = timer.getDeltaTime()*bullets[i].velocity;
			auto travel = Vec2::length(displacement);
			bullets[i].position += displacement;
			bullets[i].travelled += travel;
			if(bullets[i].travelled > 10)
			{
				bullets[i].origin += displacement;
			}

			if(bullets[i].travelled > 50)
			{
				bullets[i] = bullets.back();
				bullets.pop_back();
			}
			else
			{
				++i;
			}
		}
	}

	void killMonsters(VRayBullets& bullets, VMonsters& monsters)
	{
		std::map<uint32_t, uint32_t> kills;
		for(uint32_t b = 0; b < bullets.size();)
		{
			RayBullet& bullet = bullets[b];
			uint32_t monsterKilled = -1;
			for(uint32_t m = 0; m < monsters.size(); ++m)
			{
				Rect bbox = monsters[m].boundingBox;
				bbox.center = monsters[m].transform.position;
				if(isPointInsideRect(bullet.position, bbox))
				{
					monsterKilled = m;
					break;
				}
			}
			if(monsterKilled != -1)
			{
				monsters[monsterKilled] = monsters.back();
				monsters.pop_back();
				bullets[b] = bullets.back();
				bullets.pop_back();
				monsterKilled = -1;
			}
			else
			{
				++b;
			}
		}
	}
}
