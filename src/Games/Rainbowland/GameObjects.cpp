//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/GameObjects.h>
/******* C++ headers *******/
#include <cstdint>
#include <map>
/******* extra headers *******/
#include <Games/Rainbowland/Rainbowland.h>
#include <Graphics/Camera.h>
#include <Graphics/GraphicsSystem.h>
#include <Util/Random.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	void initGame(RainbowlandGame& game)
	{
		//background
		game.m_graphicsSystem.setBackgroundColor(0.5f, 0.5f, 0.5f);

		//players
		game.m_players.emplace_back();
		initPlayer(game.m_players.back());

		game.m_deathTimer.reset();

		//playing field boundary
		game.m_graphicsSystem.setPerspectiveProjection();

		game.m_camera.setPosition({-5.0f, 2.0f, -50.0f});
		game.m_graphicsSystem.applyCamera(game.m_camera);
		Vec2 topleft = game.m_graphicsSystem.screenToWorld({0, 0}, game.m_camera);
		game.m_playingField.halfWidth = std::abs(topleft.x);
		game.m_playingField.halfHeight = std::abs(topleft.y);

		game.m_camera.setPosition({0, 0, -50});
		game.m_graphicsSystem.setTransparencyMode(true);

		//spawners
		Rect spawnerLocations{{0, 0}, game.m_playingField.halfWidth - 3, game.m_playingField.halfHeight - 3};
		uint32_t spawnerCount = 8;
		Vec2 distanceBetweenSpawners{spawnerLocations.halfWidth * 2 / spawnerCount, spawnerLocations.halfHeight * 2 / spawnerCount};
		Vec2 pos{spawnerLocations.left(), spawnerLocations.top()};
		Random gen{Time::microsToMilis(Time::getRealTimeMicros())};
		for(uint32_t i = 0; i < spawnerCount; ++i)
		{
			game.m_monsterSpawners.emplace_back();
			game.m_monsterSpawners.back().spawnCooldown = Time::secondsToMicros(gen.randFloat() * 5 + 4);
			game.m_monsterSpawners.back().spawnRadius = 1;
			game.m_monsterSpawners.back().timer.setTimeScale(Time::NORMAL_TIME);
			game.m_monsterSpawners.back().timer.reset();
			game.m_monsterSpawners.back().transform.position = pos;

			pos.x += distanceBetweenSpawners.x;
		}
		for(uint32_t i = 0; i < spawnerCount; ++i)
		{
			game.m_monsterSpawners.emplace_back();
			game.m_monsterSpawners.back().spawnCooldown = Time::secondsToMicros(gen.randFloat() * 5 + 4);
			game.m_monsterSpawners.back().spawnRadius = 1;
			game.m_monsterSpawners.back().timer.setTimeScale(Time::NORMAL_TIME);
			game.m_monsterSpawners.back().timer.reset();
			game.m_monsterSpawners.back().transform.position = pos;

			pos.y -= distanceBetweenSpawners.y;
		}
		for(uint32_t i = 0; i < spawnerCount; ++i)
		{
			game.m_monsterSpawners.emplace_back();
			game.m_monsterSpawners.back().spawnCooldown = Time::secondsToMicros(gen.randFloat() * 5 + 4);
			game.m_monsterSpawners.back().spawnRadius = 1;
			game.m_monsterSpawners.back().timer.setTimeScale(Time::NORMAL_TIME);
			game.m_monsterSpawners.back().timer.reset();
			game.m_monsterSpawners.back().transform.position = pos;

			pos.x -= distanceBetweenSpawners.x;
		}
		for(uint32_t i = 0; i < spawnerCount; ++i)
		{
			game.m_monsterSpawners.emplace_back();
			game.m_monsterSpawners.back().spawnCooldown = Time::secondsToMicros(gen.randFloat() * 5 + 4);
			game.m_monsterSpawners.back().spawnRadius = 1;
			game.m_monsterSpawners.back().timer.setTimeScale(Time::NORMAL_TIME);
			game.m_monsterSpawners.back().timer.reset();
			game.m_monsterSpawners.back().transform.position = pos;

			pos.y += distanceBetweenSpawners.y;
		}
	}

	void cleanGame(RainbowlandGame& game)
	{
		game.m_bonuses.clear();
		game.m_monsters.clear();
		game.m_monsterSpawners.clear();
		game.m_players.clear();
		game.m_rayBullets.clear();
	}

	void initPlayer(Player& player)
	{
		player.transform.position.set(0, 0);
		player.transform.scale.set(1, 1);
		player.transform.rotation = 0;
		player.color.set(1, 0, 0);
		player.boundingBox.set(0, 0, 0.5f, 0.5f);
		player.velocity.set(0.0f, 0.0f);
		player.maxVelocity.set(5.0f, 5.0f);
		player.acceleration.set(1.0f, 1.0f);
		player.directions[Up] = player.directions[Down] = player.directions[Left] = player.directions[Right] = false;
		player.aim.set(0, 0);
		selectWeapon(player, Pistol);
		player.rateOfFireMultiplier = 1;
		player.movementSpeedMultiplier = 1;
		player.maxHealth = player.health = 100;
		player.regeneration = 0;
		player.isShooting = false;
	}

	void movePlayers(const Time& timer, VPlayers& players, const Rect& playingField)
	{
		for(Player& player : players)
		{
			Vec2 direction;
			if(player.directions[Up]) direction.y += 1;
			if(player.directions[Down]) direction.y -= 1;
			if(player.directions[Left]) direction.x -= 1;
			if(player.directions[Right]) direction.x += 1;
			player.velocity += (player.acceleration*Vec2::normalize(direction));
			direction.x = std::fabs(direction.x);
			direction.y = std::fabs(direction.y);
			player.velocity *= direction;
			clamp(-player.maxVelocity.x*player.movementSpeedMultiplier, player.maxVelocity.x*player.movementSpeedMultiplier, player.velocity.x);
			clamp(-player.maxVelocity.y*player.movementSpeedMultiplier, player.maxVelocity.y*player.movementSpeedMultiplier, player.velocity.y);
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

	void selectWeapon(Player& player, WeaponType weapon)
	{
		if(weapon == player.currentWeapon) return;

		player.currentWeapon = weapon;
		player.weaponTimer.updateBy(Time::secondsToMicros(50));
		switch(weapon)
		{
			case Pistol:
				player.weaponDelay = Time::secondsToMicros(0.5f);
				break;

			case Shotgun:
				player.weaponDelay = Time::secondsToMicros(1.0f);
				break;

			case Uzi:
				player.weaponDelay = Time::secondsToMicros(0.1f);
				break;
		}
	}

	void enableEffect(Player& player, Bonus& bonus)
	{
		auto index = filterFind(player.bonuses, [=](const BonusEffect& e){return bonus.effect == e.type; });
		if(index == player.bonuses.size())
		{
			player.bonuses.emplace_back();
			player.bonuses[index].duration = 0;
			player.bonuses[index].timer.reset();
			player.bonuses[index].type = bonus.effect;
		}
		auto& b = player.bonuses[index];
		switch(bonus.effect)
		{
			case IncreasedRateOfFire:
				player.rateOfFireMultiplier = 2;
				b.duration += Time::secondsToMicros(3);
				break;

			case IncreasedMovementSpeed:
				player.movementSpeedMultiplier = 2;
				b.duration += Time::secondsToMicros(3);
				break;

			case Heal:
				player.health += 20;
				clamp(0, 100, player.health);
				player.bonuses.pop_back();
				break;

			case WeaponDrop:
				player.bonuses.pop_back();
				selectWeapon(player, bonus.weapon);
				break;
		}
	}

	void disableEffect(Player& player, EffectType effect)
	{
		switch(effect)
		{
			case IncreasedRateOfFire:
				player.rateOfFireMultiplier = 1;
				break;

			case IncreasedMovementSpeed:
				player.movementSpeedMultiplier = 1;
				break;
		}
	}

	void updateBonusEffects(const Time& timer, VPlayers& players)
	{
		for(auto& player : players)
		{
			for(uint32_t b = 0; b < player.bonuses.size();)
			{
				auto& bonus = player.bonuses[b];
				bonus.timer.updateBy(timer.getDeltaMicros());
				if(bonus.timer.getCurMicros() > bonus.duration)
				{
					disableEffect(player, bonus.type);
					player.bonuses[b] = player.bonuses.back();
					player.bonuses.pop_back();
				}
				else
				{
					++b;
				}
			}
		}
	}

	void checkPlayerDeath(VPlayers& players)
	{
		for(uint32_t p = 0; p < players.size(); ++p)
		{
			if(players[p].health <= 0)
			{
				players[p] = players.back();
				players.pop_back();
			}
			else
			{
				++p;
			}
		}
	}

	void fireWeapon(Player& player, VRayBullets& bullets, const GraphicsSystem& graphicsSystem, const Camera& camera)
	{
		if(player.weaponTimer.getCurMicros() > (player.weaponDelay / player.rateOfFireMultiplier))
		{
			player.weaponTimer.reset();
			Vec2 normalizedAim = graphicsSystem.screenToWorld({player.aim.x, player.aim.y}, camera);
			switch(player.currentWeapon)
			{
				case Pistol:
					generateBullets(bullets, 1, 0, player.transform.position, normalizedAim, 10);
					break;

				case Shotgun:
					generateBullets(bullets, 6, 2.0f, player.transform.position, normalizedAim, 20);
					break;

				case Uzi:
					generateBullets(bullets, 1, 1.0f, player.transform.position, normalizedAim, 5);
					break;
			}
		}
	}

	void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target)
	{
		if(monsters.size() > 20) return;

		monsters.emplace_back();
		Random gen{Time::microsToMilis(Time::getRealTimeMicros())};
		auto& monster = monsters.back();
		monster.boundingBox.set(0, 0, 1, 1);
		monster.color.set(0, 0, 0);
		monster.maxVelocity = 1 + gen.randFloat() * 3;
		monster.transform.position = position;
		monster.targetPlayer = target;
		monster.maxHealth = monster.health = gen.randInt(20, 70);
	}

	void moveMonsters(const Time& timer, VMonsters& monsters, const VPlayers& players)
	{
		for(auto& monster : monsters)
		{
			if(players.size() != 0)
			{
				if(monster.targetPlayer >= players.size())
				{
					monster.targetPlayer = Random(Time::microsToMilis(Time::getRealTimeMicros())).randInt(0, players.size() - 1);
				}
				monster.direction = Vec2::normalize(players[monster.targetPlayer].transform.position - monster.transform.position);
			}
			monster.transform.position += (monster.direction*monster.maxVelocity*timer.getDeltaTime());
		}
	}

	void checkMonsterHurtingPlayer(VMonsters& monsters, VPlayers& players)
	{
		for(auto& player : players)
		{
			auto pbox = player.boundingBox;
			pbox.center = player.transform.position;
			for(auto& monster : monsters)
			{
				auto mbox = monster.boundingBox;
				mbox.center = monster.transform.position;
				if(isRectTouchingRect(mbox, pbox))
				{
					player.health -= 1;
				}
			}
		}
	}

	void updateMonsterSpawners(const Time& timer, VMonsterSpawners& spawners, VMonsters& monsters, uint32_t playerCount)
	{
		if(playerCount == 0) return;

		Random gen{Time::microsToMilis(Time::getRealTimeMicros())};
		for(auto& spawner : spawners)
		{
			spawner.timer.updateBy(timer.getDeltaMicros());
			if(spawner.timer.getCurMicros() > spawner.spawnCooldown)
			{
				spawner.timer.reset();
				generateMonster(monsters, spawner.transform.position + Vec2{(gen.randFloat() * 2 - 1)*spawner.spawnRadius, (gen.randFloat() * 2 - 1)*spawner.spawnRadius}, gen.randInt(0, playerCount - 1));
			}
		}
	}

	void generateBullets(VRayBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target, uint32_t damage)
	{

		Vec2 direction = Vec2::normalize(target - origin);
		Vec2 targetDistance = direction * 15;
		Random gen(Time::countMilisInMicros(Time::getRealTimeMicros()));
		for(uint32_t i = 0; i < count; ++i)
		{
			Vec2 t{gen.randFloat()*spread * 2 - spread, gen.randFloat()*spread * 2 - spread};
			bullets.emplace_back(RayBullet{origin, origin, Vec2::normalize(targetDistance + t) * 30, 0, damage});
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

	void generateBonuses(VKillLocations& killLocations, VBonuses& bonuses)
	{
		Random gen(Time::microsToMilis(Time::getRealTimeMicros()));
		for(auto& loc : killLocations)
		{
			if(gen.randInt(1, 10000) < 1000)
			{
				bonuses.emplace_back();
				auto effect = (EffectType)gen.randInt(0, EffectTypeCount - 1);
				bonuses.back().transform.position = loc;
				bonuses.back().boundingBox.set(0, 0, 0.75f, 0.75f);
				bonuses.back().effect = effect;
				bonuses.back().timer.reset();
				bonuses.back().duration = Time::secondsToMicros(15);
				switch(effect)
				{
					case IncreasedRateOfFire:
						bonuses.back().color.set(0, 0, 1);
						break;
					case IncreasedMovementSpeed:
						bonuses.back().color.set(0, 1, 1);
						break;
					case Heal:
						bonuses.back().color.set(0, 1, 0);
						break;
					case WeaponDrop:
						bonuses.back().weapon = (WeaponType)gen.randInt(0, WeaponCount - 1);
						bonuses.back().color.set(1, 1, 0);
						break;
				}
			}
		}
	}

	void checkBonusPickup(VPlayers& players, VBonuses& bonuses)
	{
		for(auto& player : players)
		{
			for(uint32_t b = 0; b < bonuses.size(); ++b)
			{
				Rect pbox = player.boundingBox;
				pbox.center = player.transform.position;
				Rect bbox = bonuses[b].boundingBox;
				bbox.center = bonuses[b].transform.position;
				if(isRectTouchingRect(pbox, bbox))
				{
					enableEffect(player, bonuses[b]);
					bonuses[b] = bonuses.back();
					bonuses.pop_back();
				}
			}
		}
	}

	void updateBonuses(const Time& timer, VBonuses& bonuses)
	{
		for(uint32_t b = 0; b < bonuses.size();)
		{
			bonuses[b].timer.updateBy(timer.getDeltaMicros());
			if(bonuses[b].timer.getCurMicros() > bonuses[b].duration)
			{
				bonuses[b] = bonuses.back();
				bonuses.pop_back();
			}
			else
			{
				++b;
			}
		}
	}

	void killMonsters(VRayBullets& bullets, VMonsters& monsters, VKillLocations& killLocations)
	{
		std::map<uint32_t, uint32_t> kills;
		for(uint32_t b = 0; b < bullets.size();)
		{
			RayBullet& bullet = bullets[b];
			uint32_t monsterHit = -1;
			for(uint32_t m = 0; m < monsters.size(); ++m)
			{
				Rect bbox = monsters[m].boundingBox;
				bbox.center = monsters[m].transform.position;
				if(isPointInsideRect(bullet.position, bbox))
				{
					monsterHit = m;
					break;
				}
			}
			if(monsterHit != -1)
			{
				monsters[monsterHit].health -= bullets[b].damage;
				if(monsters[monsterHit].health <= 0)
				{
					killLocations.emplace_back(monsters[monsterHit].transform.position);
					monsters[monsterHit] = monsters.back();
					monsters.pop_back();
				}
				bullets[b] = bullets.back();
				bullets.pop_back();
				monsterHit = -1;
			}
			else
			{
				++b;
			}
		}
	}
}
