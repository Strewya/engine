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
#include <Util/CollisionChecks.h>
#include <Util/Random.h>
#include <Util/Rect.h>
#include <Util/Time.h>
#include <Util/Utility.h>
#include <Window/Window.h>
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
		Rect spawnerLocations{{0, 0}, game.m_playingField.halfWidth + 3, game.m_playingField.halfHeight + 3};
		uint32_t spawnerCount = 8;
		Vec2 distanceBetweenSpawners{spawnerLocations.halfWidth * 2 / spawnerCount, spawnerLocations.halfHeight * 2 / spawnerCount};
		Vec2 pos{spawnerLocations.left(), spawnerLocations.top()};
		Random gen{Time::getRealTimeMicros()};
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

		game.m_weaponDatabase =
		{
			{Pistol, "Pew-Pew", Time::secondsToMicros(0.5f), Time::secondsToMicros(1.4f), 10, 12, 12, 1, 0, false},
			{Shotgun, "Spreader", Time::secondsToMicros(1.0f), Time::secondsToMicros(3.0f), 20, 5, 5, 6, 2.0f, false},
			{Uzi, "Urban cleaner", Time::secondsToMicros(0.1f), Time::secondsToMicros(1.3f), 5, 32, 32, 1, 0.5f, false},
			{Sniper, "One shot piercer", Time::secondsToMicros(1.4f), Time::secondsToMicros(2), 50, 8, 8, 1, 0, true},
		};
		std::sort(game.m_weaponDatabase.begin(), game.m_weaponDatabase.end(),
				  [](const Weapon& l, const Weapon& r)
		{
			return l.type < r.type;
		});

		initPerkDatabase(game.m_perkDatabase);
		initBonusDatabase(game.m_bonusDatabase);

		//players
		game.m_players.emplace_back();
		initPlayer(game.m_players.back(), game.m_weaponDatabase, game.m_perkDatabase);

		game.perkMode = false;
		game.m_gameplayTimer.reset();
		game.m_restoreTimeScaleAfterPerkMode = game.m_gameplayTimer.getTimeScale();

		game.m_defenseMatrixActive = false;
		game.m_defenseMatrixLocation.set(0, 0);
		game.m_defenseMatrixMicros = 0;
		game.m_defenseMatrixTimer.reset();
		
#ifdef _DEBUG
		grantExperience(1000, game.m_players);
#endif
	}

	void cleanGame(RainbowlandGame& game)
	{
		game.m_pickups.clear();
		game.m_monsters.clear();
		game.m_monsterSpawners.clear();
		game.m_players.clear();
		game.m_bullets.clear();
		game.m_weaponDatabase.clear();
		game.m_perkDatabase.clear();
		game.m_bonusDatabase.clear();
	}

	void initPlayer(Player& player, const VWeapons& weaponDb, const VPerks& perkDb)
	{
		player.transform.position.set(0, 0);
		player.transform.scale.set(0.5f, 0.5f);
		player.transform.rotation = 0;
		player.color.set(1, 0, 0);
		player.collisionData.set(0, 0, 1.0f);
		player.collisionData.radius *= player.transform.scale.x;
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

	void orientPlayers(VPlayers& players)
	{
		for(auto& player : players)
		{
			auto aimDir = Vec2::normalize(player.aim-player.transform.position);
			player.transform.rotation = std::atan2(aimDir.y, aimDir.x);
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
		game.m_bonusDatabase[bonus].acquireLogic(player, game);
	}

	void disableBonus(Player& player, BonusType bonus, RainbowlandGame& game)
	{
		game.m_bonusDatabase[bonus].timeoutLogic(player, game);
	}

	void updateBonuses(const Time& timer, RainbowlandGame& game)
	{
		for(auto& player : game.m_players)
		{
			for(uint32_t b = 0; b < player.bonuses.size();)
			{
				auto& bonus = player.bonuses[b];
				player.bonuses[b].timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
				if(bonus.timer.getCurrentMicros() > bonus.duration)
				{
					disableBonus(player, bonus.type, game);
				}
				else
				{
					++b;
				}
			}
		}
	}

	void generatePickups(VKillLocations& killLocations, VPickups& pickups, const VBonuses& bonusDb)
	{
		Random gen{Time::getRealTimeMicros()};
		for(auto& loc : killLocations)
		{
			if(gen.randInt(1, 10000) < 2000)
			{
				placePickup(pickups, gen, loc, (BonusType)gen.randInt(0, bonusDb.size()-1));
			}
		}
	}

	void placePickup(VPickups& pickups, Random& gen, Vec2 location, BonusType bonus)
	{
		pickups.emplace_back();
		pickups.back().transform.position = location;
		pickups.back().collisionData.set(0, 0, 0.75f);
		pickups.back().bonus = bonus;
		pickups.back().timer.reset();
		pickups.back().duration = Time::secondsToMicros(15);
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
			case Weapon_Shotgun:
			case Weapon_Uzi:
			case Weapon_Sniper:
				pickups.back().color.set(1, 1, 0);
				break;
		}
	}

	void checkPickups(VPlayers& players, RainbowlandGame& game)
	{
		for(auto& player : players)
		{
			for(uint32_t b = 0; b < game.m_pickups.size();)
			{
				Circle pCollider = player.collisionData;
				pCollider.center = player.transform.position;
				Circle bCollider = game.m_pickups[b].collisionData;
				bCollider.center = game.m_pickups[b].transform.position;
				if(isCircleTouchingCircle(pCollider, bCollider))
				{
					enableBonus(player, game.m_pickups[b].bonus, game);
					game.m_pickups[b] = game.m_pickups.back();
					game.m_pickups.pop_back();
				}
				else
				{
					++b;
				}
			}
		}
	}

	void updatePickups(const Time& timer, RainbowlandGame& game)
	{
		for(uint32_t b = 0; b < game.m_pickups.size();)
		{
			game.m_pickups[b].timer.updateBy(timer.getDeltaMicros());
			if(game.m_pickups[b].timer.getCurrentMicros() > game.m_pickups[b].duration)
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
		calculateWeaponBonuses(player, weaponDb);
		player.currentWeapon.ammo = player.currentWeapon.maxAmmo;
		player.weaponTimer.updateBy(Time::secondsToMicros(50));
	}

	void calculateWeaponBonuses(Player& player, const VWeapons& weaponDb)
	{
		const auto& dbw = weaponDb[player.currentWeapon.type];
		player.currentWeapon.damage = player.bonusDamage + dbw.damage;
		player.currentWeapon.fireDelay = static_cast<uint64_t>(player.rateOfFireMultiplier*static_cast<float>(dbw.fireDelay));
		player.currentWeapon.maxAmmo = static_cast<uint32_t>(player.ammoMultiplier*static_cast<float>(dbw.maxAmmo));
		player.currentWeapon.reloadDelay = static_cast<uint64_t>(player.reloadMultiplier*static_cast<float>(dbw.reloadDelay));
	}

	void fireWeapon(const Time& timer, Player& player, VBullets& bullets, const GraphicsSystem& graphicsSystem, const Camera& camera)
	{
		player.weaponTimer.updateBy(timer.getDeltaMicros());
		auto& w = player.currentWeapon;
		if(w.ammo > 0)
		{
			if(player.isShooting && player.weaponTimer.getCurrentMicros() > w.fireDelay)
			{
				player.weaponTimer.reset();
				--w.ammo;
				auto p = Vec2::normalize(player.aim-player.transform.position)*player.collisionData.radius;
				generateBullets(bullets, w.bulletsPerShot, w.spread, player.transform.position+p, player.aim, w.damage, w.bulletPierce);
			}
		}
		else
		{
			if(player.weaponTimer.getCurrentMicros() > w.reloadDelay)
			{
				w.ammo = w.maxAmmo;
				player.weaponTimer.updateBy(Time::secondsToMicros(50));
			}
		}
	}

	void generateBullets(VBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target, uint32_t damage, bool pierce)
	{

		Vec2 direction = Vec2::normalize(target - origin);
		Vec2 targetDistance = direction * 30;
		Random gen{Time::getRealTimeMicros()};
		for(uint32_t i = 0; i < count; ++i)
		{
			Vec2 t{gen.randFloat()*spread * 2 - spread, gen.randFloat()*spread * 2 - spread};
			bullets.emplace_back(Bullet{origin, origin, Vec2::normalize(targetDistance + t) * 60, 0, damage, pierce});
		}
	}

	void moveBullets(const Time& timer, VBullets& bullets)
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

	void findBulletHits(const Bullet& bullet, const VMonsters& monsters, std::vector<uint32_t>& outMonsters)
	{
		for(uint32_t m = 0; m < monsters.size(); ++m)
		{
			if(monsters[m].health > 0)
			{
				Circle bCollider = monsters[m].collisionData;
				bCollider.center = monsters[m].transform.position;
				if(isPointInsideCircle(bullet.position, bCollider))
				{
					outMonsters.push_back(m);
				}
			}
		}
	}

	void updateBullets(VBullets& bullets, VMonsters& monsters)
	{
		for(uint32_t b = 0; b < bullets.size();)
		{
			auto& bullet = bullets[b];
			std::vector<uint32_t> hitMonsters;
			findBulletHits(bullet, monsters, hitMonsters);
			if(!hitMonsters.empty())
			{
				std::sort(hitMonsters.begin(), hitMonsters.end(), [&](uint32_t l, uint32_t r)
				{
					auto ll = Vec2::length(monsters[l].transform.position - bullet.origin);
					auto rl = Vec2::length(monsters[r].transform.position - bullet.origin);
					return ll < rl;
				});
				for(auto mid : hitMonsters)
				{
					hurtMonster(monsters[mid], bullet.damage);
					if(!bullet.pierce)
					{
						break;
					}
				}
			}
			if(!hitMonsters.empty() && !bullet.pierce)
			{
				bullets[b] = bullets.back();
				bullets.pop_back();
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

		Random gen{Time::getRealTimeMicros()};
		for(auto& spawner : spawners)
		{
			spawner.timer.updateBy(timer.getDeltaMicros());
			if(spawner.timer.getCurrentMicros() > spawner.spawnCooldown)
			{
				spawner.timer.reset();
				generateMonster(monsters, spawner.transform.position + Vec2{(gen.randFloat() * 2 - 1)*spawner.spawnRadius, (gen.randFloat() * 2 - 1)*spawner.spawnRadius}, gen.randInt(0, playerCount - 1));
			}
		}
	}
	
	void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target)
	{
		if(monsters.size() > 40) return;

		monsters.emplace_back();
		Random gen{Time::getRealTimeMicros()};
		auto& monster = monsters.back();
		monster.collisionData.set(0, 0, 1);
		monster.maxVelocity = 1 + gen.randFloat() * 3.5f;
		monster.transform.position = position;
		monster.targetPlayer = target;
		monster.maxHealth = monster.health = gen.randInt(20, 70);
		monster.attackTimer.updateBy(Time::secondsToMicros(5));
		auto f = gen.randFloat() - 0.7f;
		if(f < 0)
		{
			f = 0;
		}
		monster.expModifier = 1 + f * 15;
		monster.color.set(f*1.2f, f*1.6f, f*1.7f);
	}

	void moveMonsters(const Time& timer, VMonsters& monsters, const VPlayers& players)
	{
		for(auto& monster : monsters)
		{
			if(players.size() != 0)
			{
				if(monster.targetPlayer >= players.size())
				{
					monster.targetPlayer = Random{Time::getRealTimeMicros()}.randInt(0, players.size() - 1);
				}
				monster.direction = Vec2::normalize(players[monster.targetPlayer].transform.position - monster.transform.position);
			}
			monster.transform.position += (monster.direction*monster.maxVelocity*timer.getDeltaTime());
		}
	}

	void orientMonsters(VMonsters& monsters)
	{
		for(auto& monster : monsters)
		{
			
			monster.transform.rotation = std::atan2(monster.direction.y, monster.direction.x);
		}
	}

	void hurtMonster(Monster& monster, uint32_t amount)
	{
		monster.health -= amount;
	}

	void checkMonsterHurtingPlayer(const Time& timer, VMonsters& monsters, VPlayers& players)
	{
		for(auto& player : players)
		{
			auto pCollider = player.collisionData;
			pCollider.center = player.transform.position;
			for(auto& monster : monsters)
			{
				auto mCollider = monster.collisionData;
				mCollider.center = monster.transform.position;
				if(isCircleTouchingCircle(mCollider, pCollider))
				{
					monster.attackTimer.updateBy(timer.getDeltaMicros());
					if(monster.attackTimer.getCurrentMicros() >= (uint64_t)Time::secondsToMicros(0.1f))
					{
						player.health -= 1;
						monster.attackTimer.reset();
					}
				}
				else
				{
					monster.attackTimer.updateBy(Time::secondsToMicros(5));
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
				auto exp = static_cast<uint32_t>(static_cast<float>(monsters[m].maxHealth)*monsters[m].expModifier);
				grantExperience(exp, players);
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
		game.m_gameplayTimer.setTimeScale(game.m_restoreTimeScaleAfterPerkMode);
		game.m_guiSystem.removeElement("perkWindow");
		if(std::all_of(game.m_players.begin(), game.m_players.end(), [](const Player& p) { return p.perkPoints == 0; }))
		{
			game.m_guiSystem.removeElement("lvlup");
		}
	}

	void generatePerks(VPlayers& players, const VPerks& perkDb)
	{
		Random gen{Time::getRealTimeMicros()};

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

	void activateDefenseMatrix(RainbowlandGame& game)
	{
		if(!game.m_defenseMatrixActive && game.m_defenseMatrixTimer.getCurrentMicros() >= game.m_defenseMatrixMicros)
		{
			game.m_defenseMatrixActive = true;
			game.m_defenseMatrixLocation = game.m_players[0].transform.position;
			game.m_defenseMatrixMicros = Time::secondsToMicros(10);
			game.m_defenseMatrixTimer.reset();
		}
	}

	void updateDefenseMatrix(RainbowlandGame& game)
	{
		game.m_defenseMatrixTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
		if(game.m_defenseMatrixActive)
		{
			Circle matrix{game.m_defenseMatrixLocation, 4};
			//push monsters out
			for(auto& monster : game.m_monsters)
			{
				Circle monstaCollider{monster.transform.position, monster.collisionData.radius};
				if(isCircleTouchingCircle(matrix, monstaCollider))
				{
					auto dir = Vec2::normalize(monstaCollider.center - matrix.center);
					monster.transform.position = matrix.center + dir*(matrix.radius + monstaCollider.radius);
				}
			}
			//check if over
			if(game.m_defenseMatrixTimer.getCurrentMicros() >= game.m_defenseMatrixMicros)
			{
				game.m_defenseMatrixTimer.reset();
				game.m_defenseMatrixMicros = Time::secondsToMicros(15);
				game.m_defenseMatrixActive = false;
			}
		}
	}
}
