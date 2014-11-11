//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/GameObjects.h>
/******* C++ headers *******/
#include <cstdint>
#include <map>
/******* extra headers *******/
#include <Games/Rainbowland/PerkDatabase.h>
#include <Games/Rainbowland/Rainbowland.h>
#include <Graphics/Camera.h>
#include <Graphics/GraphicsSystem.h>
#include <Util/CollisionChecks.h>
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

		game.m_weaponDatabase = {
				{Pistol, "Pew-Pew", Time::secondsToMicros(0.5f), Time::secondsToMicros(1.4f), 10, 12, 12, 1, 0},
				{Shotgun, "Spreader", Time::secondsToMicros(1.0f), Time::secondsToMicros(3.0f), 20, 5, 5, 6, 2.0f},
				{Uzi, "Spammer", Time::secondsToMicros(0.1f), Time::secondsToMicros(1.3f), 5, 32, 32, 1, 0.75f}
		};
		std::sort(game.m_weaponDatabase.begin(), game.m_weaponDatabase.end(),
				  [](const Weapon& l, const Weapon& r)
		{
			return l.type < r.type;
		});

		initPerkDatabase(game.m_perkDatabase);

		//players
		game.m_players.emplace_back();
		initPlayer(game.m_players.back(), game.m_weaponDatabase, game.m_perkDatabase);

		game.perkMode = false;
		
		
		game.m_players[0].perkPoints = 1;
		enterPerkMode(game);
	}

	void cleanGame(RainbowlandGame& game)
	{
		game.m_bonuses.clear();
		game.m_monsters.clear();
		game.m_monsterSpawners.clear();
		game.m_players.clear();
		game.m_rayBullets.clear();
		game.m_weaponDatabase.clear();
		game.m_perkDatabase.clear();
	}

	void initPlayer(Player& player, const VWeapons& weaponDb, const VPerks& perkDb)
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
		//player.weaponTimer set in selectWeapon
		//player.currentWeapon set in selectWeapon
		player.bonusDamage = 0;
		player.rateOfFireMultiplier = 1;
		player.movementSpeedMultiplier = 1;
		player.ammoMultiplier = 1;
		player.maxHealth = player.health = 100;
		player.regenDelayForOneHealth = 0;
		player.experience = 0;
		player.experienceForNextLevel = 200;
		player.level = 1;
		player.perkPoints = 0;
		player.perksPerLevel = 3;
		for(auto& p : perkDb)
		{
			player.availablePerks.emplace_back(p.type);
		}
		player.chosenPerk = PerkTypeCount;
		player.isShooting = false;
		selectWeapon(player, Pistol, weaponDb);
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

	void checkLevelup(VPlayers& players)
	{
		for(auto& player : players)
		{
			if(player.experience >= player.experienceForNextLevel)
			{
				player.experienceForNextLevel += 100 * player.level;
				++player.level;
				++player.perkPoints;
			}
		}
	}

	void enableEffect(Player& player, Bonus& bonus, const VWeapons& weaponDb)
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
				b.duration += Time::secondsToMicros(10);
				break;

			case IncreasedMovementSpeed:
				player.movementSpeedMultiplier = 2;
				b.duration += Time::secondsToMicros(5);
				break;

			case Heal:
				player.health += 20;
				clamp(0, 100, player.health);
				player.bonuses.pop_back();
				break;

			case WeaponDrop:
				player.bonuses.pop_back();
				selectWeapon(player, bonus.weapon, weaponDb);
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

	void checkBonusPickup(VPlayers& players, VBonuses& bonuses, const VWeapons& weaponDb)
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
					enableEffect(player, bonuses[b], weaponDb);
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

	void selectWeapon(Player& player, WeaponType weapon, const VWeapons& weaponDb)
	{
		player.currentWeapon = weaponDb[weapon];
		player.currentWeapon.ammo = static_cast<uint32_t>(player.ammoMultiplier*static_cast<float>(player.currentWeapon.ammo));
		player.currentWeapon.maxAmmo = player.currentWeapon.ammo;
		player.currentWeapon.damage += player.bonusDamage;
		player.weaponTimer.updateBy(Time::secondsToMicros(50));
	}

	void fireWeapon(const Time& timer, Player& player, VRayBullets& bullets, const GraphicsSystem& graphicsSystem, const Camera& camera)
	{
		player.weaponTimer.updateBy(timer.getDeltaMicros());
		if(player.isShooting)
		{
			auto& w = player.currentWeapon;
			if(w.ammo > 0)
			{
				if(player.weaponTimer.getCurMicros() > (w.fireDelay / player.rateOfFireMultiplier))
				{
					player.weaponTimer.reset();
					--w.ammo;
					Vec2 normalizedAim = graphicsSystem.screenToWorld({player.aim.x, player.aim.y}, camera);
					generateBullets(bullets, w.bulletsPerShot, w.spread, player.transform.position, normalizedAim, w.damage);
				}
			}
			else
			{
				if(player.weaponTimer.getCurMicros() > (w.reloadDelay/player.rateOfFireMultiplier))
				{
					w.ammo = w.maxAmmo;
					player.weaponTimer.updateBy(Time::secondsToMicros(50));
				}
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

	void checkBulletHits(VRayBullets& bullets, VMonsters& monsters)
	{
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
	
	void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target)
	{
		if(monsters.size() > 20) return;

		monsters.emplace_back();
		Random gen{Time::microsToMilis(Time::getRealTimeMicros())};
		auto& monster = monsters.back();
		monster.boundingBox.set(0, 0, 1, 1);
		monster.color.set(0, 0, 0);
		monster.maxVelocity = 1 + gen.randFloat() * 2;
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

	void killMonsters(VMonsters& monsters, VKillLocations& killLocations, VPlayers& players)
	{
		for(uint32_t m = 0; m < monsters.size(); )
		{
			if(monsters[m].health <= 0)
			{
				for(auto& player : players)
				{
					player.experience += monsters[m].maxHealth;
				}
				killLocations.emplace_back(monsters[m].transform.position);
				monsters[m] = monsters.back();
				monsters.pop_back();
			}
			else
			{
				++m;
			}
		}
	}

	bool enterPerkMode(RainbowlandGame& game)
	{
		if(std::any_of(game.m_players.begin(), game.m_players.end(), [](const Player& p){return p.perkPoints > 0; }))
		{
			for(auto& player : game.m_players)
			{
				player.directions[Up] = player.directions[Down] = player.directions[Left] = player.directions[Right] = false;
			}
			game.perkMode = true;
			generatePerks(game.m_players, game.m_perkDatabase);
			game.m_gameplayTimer.setTimeScale(Time::STOP_TIME);
			//setup the gui system and add the required buttons
			game.m_guiSystem.panel("perkWindow", "root", {}, {300, 300}, {0.1f, 0.1f, 0.1f});
			uint32_t playerIndex = 0, perkIndex = 0;
			float row = 250;
			for(auto& player : game.m_players)
			{
				float distance = 600.0f / (float)player.selectablePerks.size();
				float column = -300 + distance*0.5f;
				for(auto& perk : player.selectablePerks)
				{
					auto name = "player" + std::to_string(playerIndex) + "perk" + std::to_string(perkIndex);
					auto textSize = game.m_graphicsSystem.textSize(game.m_defaultFont, game.m_perkDatabase[perk].name);
					textSize *= Vec2{0.5f, 0.5f};
					game.m_guiSystem.button(name, "perkWindow", {column, row}, {(distance - 10)*0.5f, textSize.y + 10}, {1, 1, 1},
											Mouse::m_LeftButton, [=, &player, &game]()
					{
						player.chosenPerk = perk;
						if(allPlayersChosePerk(game.m_players))
						{
							applyPerksForPlayers(game);
							exitPerkMode(game);
						}
					});
					game.m_guiSystem.label(name + "Label", name, game.m_defaultFont, game.m_perkDatabase[perk].name, {}, {0.5f, 0.5f}, {1, 1, 1}, 1, false);
					++perkIndex;
					column += distance;
				}
				++playerIndex;
				row -= 80;
			}
			auto textSize = game.m_graphicsSystem.textSize(game.m_defaultFont, "cancel");
			Vec2 textScale{0.5f, 0.5f};
			textSize *= textScale;
			game.m_guiSystem.button("close", "perkWindow", {300 - textSize.x - 10, -300 + textSize.y + 10}, textSize, {1,1,1,1},
									Mouse::m_LeftButton, [&game]()
			{
				exitPerkMode(game);
			});
			game.m_guiSystem.label("closeLabel", "close", game.m_defaultFont, "close", {}, textScale, {1, 1, 1}, 1, false);
			return true;
		}
		return false;
	}

	void exitPerkMode(RainbowlandGame& game)
	{
		game.perkMode = false;
		game.m_gameplayTimer.setTimeScale(Time::NORMAL_TIME);
		game.m_guiSystem.removeElement("perkWindow");
	}

	void generatePerks(VPlayers& players, const VPerks& perkDb)
	{
		Random gen(Time::microsToMilis(Time::getRealTimeMicros()));

		for(auto& player : players)
		{
#ifdef _DEBUG
			player.selectablePerks.clear();
#else
			if(!player.selectablePerks.empty()) continue;
#endif
			player.chosenPerk = PerkTypeCount;
			auto perks = player.availablePerks;
			for(uint32_t i = 0; i < player.perksPerLevel; ++i)
			{
				uint32_t perkIndex = gen.randInt(0, perks.size() - 1);
				player.selectablePerks.emplace_back(perks[perkIndex]);
				perks[perkIndex] = perks.back();
				perks.pop_back();
			}
		}
	}

	bool allPlayersChosePerk(VPlayers& players)
	{
		bool allChose = true;
		for(auto& player : players)
		{
			allChose = allChose && (player.chosenPerk != PerkTypeCount);
		}
		return allChose;
	}

	void applyPerksForPlayers(RainbowlandGame& game)
	{
		for(auto& player : game.m_players)
		{
			--player.perkPoints;
			player.selectablePerks.clear();
			player.acquiredPerks.emplace_back(player.chosenPerk);
			auto index = valueFind(player.availablePerks, player.chosenPerk);
			player.availablePerks.erase(player.availablePerks.begin() + index);
			game.m_perkDatabase[player.chosenPerk].acquireLogic(player, game);
		}
	}

	void updatePerks(RainbowlandGame& game)
	{
		for(auto& player : game.m_players)
		{
			for(auto perk : player.acquiredPerks)
			{
				game.m_perkDatabase[perk].updateLogic(player, game);
			}
		}
	}
}
