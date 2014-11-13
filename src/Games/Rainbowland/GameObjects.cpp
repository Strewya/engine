//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/GameObjects.h>
/******* C++ headers *******/
#include <cstdint>
#include <map>
/******* extra headers *******/
#include <Games/Rainbowland/BonusDatabase.h>
#include <Games/Rainbowland/PerkDatabase.h>
#include <Games/Rainbowland/Rainbowland.h>
#include <Graphics/Camera.h>
#include <Graphics/GraphicsSystem.h>
#include <Util/Clock.h>
#include <Util/CollisionChecks.h>
#include <Util/Random.h>
#include <Util/Rect.h>
#include <Util/Timers.h>
#include <Util/Utility.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
	void initGame(RainbowlandGame& game)
	{
		//background
		game.m_graphicsSystem.setBackgroundColor(0.5f, 0.5f, 0.5f);

		game.m_deathTimer.resetToStartingValue();

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
		Random gen{Clock::getRealTimeMicros()};
		for(uint32_t i = 0; i < spawnerCount; ++i)
		{
			game.m_monsterSpawners.emplace_back();
			game.m_monsterSpawners.back().spawnRadius = 1;
			game.m_monsterSpawners.back().spawnTimer.setTimeScale(Clock::NORMAL_TIME);
			game.m_monsterSpawners.back().spawnTimer.setIntervalSeconds(gen.randFloat() * 5 + 4);
			game.m_monsterSpawners.back().transform.position = pos;

			pos.x += distanceBetweenSpawners.x;
		}
		for(uint32_t i = 0; i < spawnerCount; ++i)
		{
			game.m_monsterSpawners.emplace_back();
			game.m_monsterSpawners.back().spawnRadius = 1;
			game.m_monsterSpawners.back().spawnTimer.setTimeScale(Clock::NORMAL_TIME);
			game.m_monsterSpawners.back().spawnTimer.setIntervalSeconds(gen.randFloat() * 5 + 4);
			game.m_monsterSpawners.back().transform.position = pos;

			pos.y -= distanceBetweenSpawners.y;
		}
		for(uint32_t i = 0; i < spawnerCount; ++i)
		{
			game.m_monsterSpawners.emplace_back();
			game.m_monsterSpawners.back().spawnRadius = 1;
			game.m_monsterSpawners.back().spawnTimer.setTimeScale(Clock::NORMAL_TIME);
			game.m_monsterSpawners.back().spawnTimer.setIntervalSeconds(gen.randFloat() * 5 + 4);
			game.m_monsterSpawners.back().transform.position = pos;

			pos.x -= distanceBetweenSpawners.x;
		}
		for(uint32_t i = 0; i < spawnerCount; ++i)
		{
			game.m_monsterSpawners.emplace_back();
			game.m_monsterSpawners.back().spawnRadius = 1;
			game.m_monsterSpawners.back().spawnTimer.setTimeScale(Clock::NORMAL_TIME);
			game.m_monsterSpawners.back().spawnTimer.setIntervalSeconds(gen.randFloat() * 5 + 4);
			game.m_monsterSpawners.back().transform.position = pos;

			pos.y += distanceBetweenSpawners.y;
		}

		game.m_weaponDatabase = {
				{Pistol, "Pew-Pew", Clock::secondsToMicros(0.5f), Clock::secondsToMicros(1.4f), 10, 12, 12, 1, 0},
				{Shotgun, "Spreader", Clock::secondsToMicros(1.0f), Clock::secondsToMicros(3.0f), 20, 5, 5, 6, 2.0f},
				{Uzi, "Spammer", Clock::secondsToMicros(0.1f), Clock::secondsToMicros(1.3f), 5, 32, 32, 1, 0.5f}
		};
		std::sort(game.m_weaponDatabase.begin(), game.m_weaponDatabase.end(),
				  [](const Weapon& l, const Weapon& r)
		{
			return l.type < r.type;
		});

		initBonusDatabase(game.m_bonusDatabase);
		initPerkDatabase(game.m_perkDatabase);

		//players
		game.m_players.emplace_back();
		initPlayer(game.m_players.back(), game.m_weaponDatabase, game.m_perkDatabase);

		game.perkMode = false;
		game.m_restoreTimeScaleAfterPerkMode = game.m_gameplayTimer.getTimeScale();
		
#ifdef _DEBUG
		grantExperience(1000, game.m_players);
#endif
	}

	void cleanGame(RainbowlandGame& game)
	{
		game.m_players.clear();
		game.m_monsters.clear();
		game.m_monsterSpawners.clear();
		game.m_pickups.clear();
		game.m_rayBullets.clear();
		game.m_weaponDatabase.clear();
		game.m_perkDatabase.clear();
		game.m_bonusDatabase.clear();
	}

	void initPlayer(Player& player, const VWeapons& weaponDb, const VPerks& perkDb)
	{
		player.transform.position.set(0, 0);
		player.transform.scale.set(1, 1);
		player.transform.rotation = 0;
		player.color.set(1, 0, 0);
		player.collisionData.set(0, 0, 0.5f);
		player.velocity.set(0.0f, 0.0f);
		player.maxVelocity.set(5.0f, 5.0f);
		player.acceleration.set(1.0f, 1.0f);
		player.directions[Up] = player.directions[Down] = player.directions[Left] = player.directions[Right] = false;
		player.aim.set(0, 0);
		//player.weaponTimer set in selectWeapon
		//player.currentWeapon set in selectWeapon
		player.bonusDamage = 0;
		player.rateOfFireMultiplier = 1;
		player.reloadMultiplier = 1;
		player.ammoMultiplier = 1;
		player.maxHealth = player.health = 100;
		player.regenDelayForOneHealth = 0;
		player.experience = 0;
		player.experienceForNextLevel = 1000;
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

	void movePlayers(const IncrementingTimer& timer, VPlayers& players, const Rect& playingField)
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
			clamp(-player.maxVelocity.x, player.maxVelocity.x, player.velocity.x);
			clamp(-player.maxVelocity.y, player.maxVelocity.y, player.velocity.y);
			player.transform.position += (timer.getDeltaTime()*player.velocity);
			Circle playerCollider = player.collisionData;
			playerCollider.center = player.transform.position;
			if(!isCircleInsideRect(playerCollider, playingField))
			{
				clamp(playingField.left() + player.collisionData.radius, playingField.right() - player.collisionData.radius, player.transform.position.x);
				clamp(playingField.bottom() + player.collisionData.radius, playingField.top() - player.collisionData.radius, player.transform.position.y);
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

	void checkLevelup(VPlayers& players, RainbowlandGame& game)
	{
		for(auto& player : players)
		{
			if(player.experience >= player.experienceForNextLevel)
			{
				++player.level;
				++player.perkPoints;
				player.experienceForNextLevel += 1000 * player.level;
				if(player.perkPoints == 1)
				{
					auto textSize = game.m_graphicsSystem.textSize(game.m_defaultFont, "Level");
					textSize *= 0.5f;
					Rect r;
					r.center.set(game.m_window->getSizeX()*0.5f - textSize.x - 10, 200);
					r.halfHeight = textSize.y;
					r.halfWidth = textSize.x + 10;
					game.m_guiSystem.panel("lvlup", "root", r.center, {r.halfWidth, r.halfHeight}, {0, 0, 0});
					game.m_guiSystem.label("lvlupLabel", "lvlup", game.m_defaultFont, "Level", {}, {0.5f, 0.5f}, {}, 1, false);
				}
			}
		}
	}

	void grantExperience(uint32_t exp, VPlayers& players)
	{
		for(auto& player : players)
		{
			player.experience += exp;
		}
	}

	void enableBonus(Player& player, BonusType bonus, RainbowlandGame& game)
	{
		auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return bonus == e.type; });
		bool exists = index != player.bonuses.size();
		if(!exists)
		{
			player.bonuses.emplace_back();
			player.bonuses[index].type = bonus;
		}
		auto& b = player.bonuses[index];

		game.m_bonusDatabase[b.type].acquireLogic(player, game);
		/*
		switch(bonus)
		{
			case IncreasedRateOfFire:
				if(!exists)
				{
					player.rateOfFireMultiplier *= 0.5f;
					player.reloadMultiplier *= 0.5f;
					player.currentWeapon.fireDelay = static_cast<uint64_t>(0.5f*static_cast<float>(player.currentWeapon.fireDelay));
					player.currentWeapon.reloadDelay = static_cast<uint64_t>(0.5f*static_cast<float>(player.currentWeapon.reloadDelay));
				}
				b.timer.addExtraMicros(Clock::secondsToMicros(game.m_bonusDatabase[bonus].duration));
				break;

			case IncreasedMovementSpeed:
				if(!exists)
				{
					player.maxVelocity *= 2;
				}
				b.timer.addExtraMicros(Clock::secondsToMicros(game.m_bonusDatabase[bonus].duration));
				break;

			case Heal:
				player.health += 20;
				clamp(0, 100, player.health);
				player.bonuses.pop_back();
				break;

			case SlowTime:
				if(!exists)
				{
					game.m_timeScaleForGameplayTimer.emplace_back(game.m_gameplayTimer.getTimeScale() * 0.4);
				}
				b.timer.addExtraMicros(Clock::secondsToMicros(game.m_bonusDatabase[bonus].duration));
				break;

			case Weapon_Pistol:
				player.bonuses.pop_back();
				selectWeapon(player, Pistol, game.m_weaponDatabase);
				break;
			case Weapon_Shotgun:
				player.bonuses.pop_back();
				selectWeapon(player, Shotgun, game.m_weaponDatabase);
				break;
			case Weapon_Uzi:
				player.bonuses.pop_back();
				selectWeapon(player, Uzi, game.m_weaponDatabase);
				break;
		}*/
	}

	void disableEffect(Player& player, BonusType bonus, RainbowlandGame& game)
	{
		game.m_bonusDatabase[bonus].timeoutLogic(player, game);
		/*
		switch(effect)
		{
			case IncreasedRateOfFire:
				player.rateOfFireMultiplier *= 2;
				player.reloadMultiplier *= 2;
				player.currentWeapon.fireDelay = static_cast<uint64_t>(2*static_cast<float>(player.currentWeapon.fireDelay));
				player.currentWeapon.reloadDelay = static_cast<uint64_t>(2*static_cast<float>(player.currentWeapon.reloadDelay));
				break;
	
			case IncreasedMovementSpeed:
				player.maxVelocity *= 0.5f;
				break;

			case SlowTime:
				game.m_gameplayTimer.setTimeScale(game.m_gameplayTimer.getTimeScale() / 0.4);
				break;
		}*/
	}

	void updateBonuses(const IncrementingTimer& timer, RainbowlandGame& game)
	{
		for(auto& player : game.m_players)
		{
			for(uint32_t b = 0; b < player.bonuses.size();)
			{
				auto& bonus = player.bonuses[b];
				if(bonus.type == SlowTime) bonus.timer.updateBy(game.m_logicClock.getDeltaMicros());
				else bonus.timer.updateBy(timer.getDeltaMicros());
				if(bonus.timer.isDone())
				{
					disableEffect(player, bonus.type, game);
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

	void generatePickups(VKillLocations& killLocations, VPickups& pickups)
	{
		Random gen(Clock::getRealTimeMicros());
		for(auto& loc : killLocations)
		{
			if(gen.randInt(1, 10000) < 2000)
			{
				placePickup(pickups, gen, loc, (BonusType)gen.randInt(0, BonusTypeCount - 1));
			}
		}
	}

	void placePickup(VPickups& pickups, Random& gen, Vec2 location, BonusType bonus)
	{
		pickups.emplace_back();
		pickups.back().transform.position = location;
		pickups.back().collisionData.set(0, 0, 0.75f);
		pickups.back().type = bonus;
		pickups.back().timer.startSeconds(15);
		switch(bonus)
		{
			case IncreasedRateOfFire:
				pickups.back().color.set(0, 0, 1);
				break;
			case IncreasedMovementSpeed:
				pickups.back().color.set(0, 1, 1);
				break;
			case Heal:
				pickups.back().color.set(0, 1, 0);
				break;
			case SlowTime:
				pickups.back().color.set(1, 0.5f, 1);
				break;
			case Weapon_Pistol:
				pickups.back().color.set(1, 1, 0);
				break;
			case Weapon_Shotgun:
				pickups.back().color.set(1, 1, 0);
				break;
			case Weapon_Uzi:
				pickups.back().color.set(1, 1, 0);
				break;
		}
	}

	void checkPickups(VPlayers& players, RainbowlandGame& game)
	{
		for(auto& player : players)
		{
			for(uint32_t b = 0; b < game.m_pickups.size(); ++b)
			{
				Circle pCollider = player.collisionData;
				pCollider.center = player.transform.position;
				Circle bCollider = game.m_pickups[b].collisionData;
				bCollider.center = game.m_pickups[b].transform.position;
				if(isCircleTouchingCircle(pCollider, bCollider))
				{
					enableBonus(player, game.m_pickups[b].type, game);
					game.m_pickups[b] = game.m_pickups.back();
					game.m_pickups.pop_back();
				}
			}
		}
	}

	void updatePickups(const IncrementingTimer& timer, RainbowlandGame& game)
	{
		for(uint32_t b = 0; b < game.m_pickups.size();)
		{
			game.m_pickups[b].timer.updateBy(timer.getDeltaMicros());
			if(game.m_pickups[b].timer.isDone())
			{
				game.m_pickups[b] = game.m_pickups.back();
				game.m_pickups.pop_back();
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
		player.currentWeapon.maxAmmo = static_cast<uint32_t>(player.ammoMultiplier*static_cast<float>(player.currentWeapon.maxAmmo));
		player.currentWeapon.ammo = player.currentWeapon.maxAmmo;
		player.currentWeapon.damage += player.bonusDamage;
		player.currentWeapon.fireDelay = static_cast<uint64_t>(player.rateOfFireMultiplier*static_cast<float>(player.currentWeapon.fireDelay));
		player.currentWeapon.reloadDelay = static_cast<uint64_t>(player.reloadMultiplier*static_cast<float>(player.currentWeapon.reloadDelay));
		player.attackTimer.setIntervalMicros(player.currentWeapon.fireDelay);
		player.attackTimer.updateBy(player.currentWeapon.fireDelay);
		player.reloadTimer.startMicros(player.currentWeapon.reloadDelay);
	}

	void fireWeapon(const IncrementingTimer& timer, Player& player, VRayBullets& bullets, const GraphicsSystem& graphicsSystem, const Camera& camera)
	{
		auto& w = player.currentWeapon;
		if(w.ammo > 0)
		{
			player.attackTimer.updateBy(timer.getDeltaMicros());
			if(player.isShooting && player.attackTimer.isIntervalPassed())
			{
				--w.ammo;
				Vec2 worldPosAim = graphicsSystem.screenToWorld({player.aim.x, player.aim.y}, camera);
				generateBullets(bullets, w.bulletsPerShot, w.spread, player.transform.position, worldPosAim, w.damage);
			}
		}
		else
		{
			player.reloadTimer.updateBy(timer.getDeltaMicros());
			if(player.reloadTimer.isDone())
			{
				player.reloadTimer.resetToStartingValue();
				w.ammo = w.maxAmmo;
				player.attackTimer.updateBy(player.currentWeapon.reloadDelay);
			}
		}
	}

	void generateBullets(VRayBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target, uint32_t damage)
	{

		Vec2 direction = Vec2::normalize(target - origin);
		Vec2 targetDistance = direction * 30;
		Random gen(Clock::getRealTimeMicros());
		for(uint32_t i = 0; i < count; ++i)
		{
			Vec2 t{gen.randFloat()*spread * 2 - spread, gen.randFloat()*spread * 2 - spread};
			bullets.emplace_back(RayBullet{origin, origin, Vec2::normalize(targetDistance + t) * 30, 0, damage});
		}
	}

	void moveBullets(const IncrementingTimer& timer, VRayBullets& bullets)
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
				Circle bCollider = monsters[m].collisionData;
				bCollider.center = monsters[m].transform.position;
				if(isPointInsideCircle(bullet.position, bCollider))
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

	void updateMonsterSpawners(const IncrementingTimer& timer, VMonsterSpawners& spawners, VMonsters& monsters, uint32_t playerCount)
	{
		if(playerCount == 0) return;

		Random gen{Clock::getRealTimeMicros()};
		for(auto& spawner : spawners)
		{
			spawner.spawnTimer.updateBy(timer.getDeltaMicros());
			if(spawner.spawnTimer.isIntervalPassed())
			{
				generateMonster(monsters, spawner.transform.position + Vec2{(gen.randFloat() * 2 - 1)*spawner.spawnRadius, (gen.randFloat() * 2 - 1)*spawner.spawnRadius}, gen.randInt(0, playerCount - 1));
			}
		}
	}
	
	void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target)
	{
		if(monsters.size() > 40) return;

		monsters.emplace_back();
		Random gen{Clock::getRealTimeMicros()};
		auto& monster = monsters.back();
		monster.collisionData.set(0, 0, 1);
		monster.color.set(0, 0, 0);
		monster.maxVelocity = 1 + gen.randFloat() * 3.5f;
		monster.transform.position = position;
		monster.targetPlayer = target;
		monster.maxHealth = monster.health = gen.randInt(20, 70);
		monster.attackTimer.setIntervalSeconds(0.3f);
		monster.attackTimer.updateBy(Clock::secondsToMicros(0.3f));
	}

	void moveMonsters(const IncrementingTimer& timer, VMonsters& monsters, const VPlayers& players)
	{
		for(auto& monster : monsters)
		{
			if(players.size() != 0)
			{
				if(monster.targetPlayer >= players.size())
				{
					monster.targetPlayer = Random(Clock::getRealTimeMicros()).randInt(0, players.size() - 1);
				}
				monster.direction = Vec2::normalize(players[monster.targetPlayer].transform.position - monster.transform.position);
			}
			monster.transform.position += (monster.direction*monster.maxVelocity*timer.getDeltaTime());
		}
	}

	void checkMonsterHurtingPlayer(const IncrementingTimer& timer, VMonsters& monsters, VPlayers& players)
	{
		for(auto& player : players)
		{
			auto pCollider = player.collisionData;
			pCollider.center = player.transform.position;
			for(auto& monster : monsters)
			{
				auto mCollider = monster.collisionData;
				mCollider.center = monster.transform.position;
				monster.attackTimer.updateBy(timer.getDeltaMicros());
				if(isCircleTouchingCircle(mCollider, pCollider))
				{
					if(monster.attackTimer.isIntervalPassed())
					{
						player.health -= 1;
					}
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
				grantExperience(monsters[m].maxHealth, players);
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
			game.m_restoreTimeScaleAfterPerkMode = game.m_gameplayTimer.getTimeScale();
			game.m_gameplayTimer.setTimeScale(Clock::STOP_TIME);
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
		game.m_gameplayTimer.setTimeScale(game.m_restoreTimeScaleAfterPerkMode);
		game.m_guiSystem.removeElement("perkWindow");
		if(std::all_of(game.m_players.begin(), game.m_players.end(), [](const Player& p) { return p.perkPoints == 0; }))
		{
			game.m_guiSystem.removeElement("lvlup");
		}
	}

	void generatePerks(VPlayers& players, const VPerks& perkDb)
	{
		Random gen(Clock::getRealTimeMicros());

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
				if(perks.size() != 0)
				{
					uint32_t perkIndex = gen.randInt(0, perks.size() - 1);
					player.selectablePerks.emplace_back(perks[perkIndex]);
					perks[perkIndex] = perks.back();
					perks.pop_back();
				}
				else
				{
					player.selectablePerks.emplace_back(InstantWinner);
				}
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
			if(player.availablePerks.size() > 0)
			{
				auto index = valueFind(player.availablePerks, player.chosenPerk);
				player.availablePerks.erase(player.availablePerks.begin() + index);
			}
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
