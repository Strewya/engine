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
		game.m_window->showCursor(false);
		game.m_window->lockCursor(true);
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

		game.m_graphicsSystem.createTextureRenderTarget(1200, 900);

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
		initPlayer(game.m_players.back(), 0, game);
		//game.m_players.emplace_back();
		//initPlayer(game.m_players.back(), 1, game);
		//game.m_players.back().color.set(0, 0, 1);

		game.perkMode = false;
		game.m_gameplayTimer.reset();
		game.m_restoreTimeScaleAfterPerkMode = game.m_gameplayTimer.getTimeScale();

		game.m_defenseMatrixActive = false;
		game.m_defenseMatrixArea.set(0, 0, 4);
		game.m_defenseMatrixMicros = 0;
		game.m_defenseMatrixTimer.reset();

		game.m_timeCapsuleActive = false;
		game.m_timeCapsuleArea.set(0, 0, 10);
		game.m_timeCapsuleMicros = 0;
		game.m_timeCapsuleTimer.reset();

		
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
		game.m_graphicsSystem.clearTextureRenderTarget();
	}

	void initPlayer(Player& player, uint32_t id, RainbowlandGame& game)
	{
		player.id = id;
		player.objectTimer.reset();
		player.transform.position.set(0+(float)id, 0);
		player.transform.scale.set(1.0f, 1.0f);
		player.transform.rotation = 0;
		player.color.r = id == 0 ? 1.0f : 0.0f;
		player.color.g = id == 1 ? 1.0f : 0.0f;
		player.color.b = id == 2 ? 1.0f : 0.0f;
		player.collisionData.set(0, 0, 1.0f);
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
		for(auto& p : game.m_perkDatabase)
		{
			player.availablePerks.emplace_back(p.type);
		}
		player.chosenPerk = PerkTypeCount;
		player.isShooting = false;
		selectWeapon(player, Pistol, game.m_weaponDatabase);
		//gui
		auto panel = "player" + std::to_string(id);

		auto health = "Health: " + std::to_string(player.health) + "/" + std::to_string(player.maxHealth);
		Vec2 healthPos{5 - 0.5f*game.m_window->getSizeX(), 0.5f*game.m_window->getSizeY() - (id+1)*20.0f};
		
		auto exp = "Exp/next level: " + std::to_string(player.experience) + "/" + std::to_string(player.experienceForNextLevel);
		Vec2 expPos = healthPos;
		expPos.x += game.m_graphicsSystem.textHalfSize(game.m_defaultFont, health).x * 2 * 0.75f + 50;
		
		auto ammo = "Ammo: " + std::to_string(player.currentWeapon.ammo) + "/" + std::to_string(player.currentWeapon.maxAmmo);
		Vec2 ammoPos = expPos;
		ammoPos.x += game.m_graphicsSystem.textHalfSize(game.m_defaultFont, exp).x * 2;
		
		auto weapon = "Weapon: " + player.currentWeapon.name;
		Vec2 weaponPos = ammoPos;
		weaponPos.x += game.m_graphicsSystem.textHalfSize(game.m_defaultFont, ammo).x * 2 * 0.75f + 50;
		
		game.m_guiSystem.panel(panel, "root", {0, 0}, {(float)game.m_window->getSizeX(), (float)game.m_window->getSizeY()}, {0, 0, 0, 0});
		game.m_guiSystem.label(panel + "HP", panel, game.m_defaultFont, health, healthPos, {0.75f, 0.75f}, {0, 0, 0}, TJ_Left, false);
		game.m_guiSystem.label(panel + "EXP", panel, game.m_defaultFont, exp, expPos, {0.75f, 0.75f}, {0, 0, 0}, TJ_Left, false);
		game.m_guiSystem.label(panel + "AMMO", panel, game.m_defaultFont, ammo, ammoPos, {0.75f, 0.75f}, {0, 0, 0}, TJ_Left, false);
		game.m_guiSystem.label(panel + "WEAPON", panel, game.m_defaultFont, weapon, weaponPos, {0.75f, 0.75f}, {0, 0, 0}, TJ_Left, false);
	}

	void movePlayers(VPlayers& players, const Rect& playingField)
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
			player.transform.position += (player.objectTimer.getDeltaTime()*player.velocity);
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

	void checkPlayerDeath(RainbowlandGame& game)
	{
		for(uint32_t p = 0; p < game.m_players.size();)
		{
			if(game.m_players[p].health <= 0)
			{
				game.m_guiSystem.removeElement("player" + std::to_string(game.m_players[p].id));
				game.m_players[p] = game.m_players.back();
				game.m_players.pop_back();
			}
			else
			{
				++p;
			}
		}
	}

	void checkLevelup(RainbowlandGame& game)
	{
		for(auto& player : game.m_players)
		{
			if(player.experience >= player.experienceForNextLevel)
			{
				++player.level;
				++player.perkPoints;
				player.experienceForNextLevel += 1000 * player.level;
				if(player.perkPoints == 1)
				{
					auto textSize = game.m_graphicsSystem.textHalfSize(game.m_defaultFont, "Level");
					textSize *= 0.5f;
					Rect r;
					r.center.set(game.m_window->getSizeX()*0.5f - textSize.x - 10, 200);
					r.halfHeight = textSize.y;
					r.halfWidth = textSize.x + 10;
					game.m_guiSystem.panel("lvlup", "root", r.center, {r.halfWidth, r.halfHeight}, {0, 0, 0});
					game.m_guiSystem.label("lvlupLabel", "lvlup", game.m_defaultFont, "Level", {}, {0.5f, 0.5f}, {}, TJ_Center, false);
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

	void updateGuiLabels(RainbowlandGame& game)
	{
		for(auto& player : game.m_players)
		{
			auto exp = "Exp/next level: " + std::to_string(player.experience) + "/" + std::to_string(player.experienceForNextLevel);
			game.m_guiSystem.editLabel("player" + std::to_string(player.id) + "EXP", exp);

			auto weapon = "Weapon: " + player.currentWeapon.name;
			game.m_guiSystem.editLabel("player" + std::to_string(player.id) + "WEAPON", weapon);

			auto ammo = "Ammo: " + std::to_string(player.currentWeapon.ammo) + "/" + std::to_string(player.currentWeapon.maxAmmo);
			game.m_guiSystem.editLabel("player" + std::to_string(player.id) + "AMMO", ammo);

			auto health = "Health: " + std::to_string(player.health) + "/" + std::to_string(player.maxHealth);
			game.m_guiSystem.editLabel("player" + std::to_string(player.id) + "HP", health);
		}
	}

	void fixupCamera(RainbowlandGame& game)
	{
		if(game.m_players.size() > 0)
		{
			Vec2 averagePos;
			for(auto& player : game.m_players)
			{
				averagePos += player.transform.position;
			}
			averagePos /= (float)game.m_players.size();
			auto pos = game.m_camera.getPosition();
			Vec2 pos2{pos.x, pos.y};
			clamp(-5.0f, 5.0f, averagePos.x);
			clamp(-2.0f, 2.0f, averagePos.y);
			auto diff = averagePos - pos2;
			if(Vec2::length(diff) > 0)
			{
				pos.set(averagePos.x, averagePos.y, pos.z);
				game.m_camera.move({diff.x, diff.y, 0});
				game.m_players[0].aim += diff;
			}
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

	void updateBonuses(RainbowlandGame& game)
	{
		for(auto& player : game.m_players)
		{
			for(uint32_t b = 0; b < player.bonuses.size();)
			{
				auto& bonus = player.bonuses[b];
				player.bonuses[b].timer.updateBy(player.objectTimer.getDeltaMicros());
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
		pickups.back().objectTimer.reset();
		pickups.back().transform.position = location;
		pickups.back().collisionData.set(0, 0, 0.75f);
		pickups.back().bonus = bonus;
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

	void checkPickups(RainbowlandGame& game)
	{
		for(auto& player : game.m_players)
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

	void updatePickups(RainbowlandGame& game)
	{
		for(uint32_t b = 0; b < game.m_pickups.size();)
		{
			if(game.m_pickups[b].objectTimer.getCurrentMicros() > game.m_pickups[b].duration)
			{
				game.m_pickups[b] = game.m_pickups.back();
				game.m_pickups.pop_back();
			}
			else
			{
				auto duration = game.m_pickups[b].duration;
				auto diff = duration - game.m_pickups[b].objectTimer.getCurrentMicros();
				game.m_pickups[b].color.a = 0.2f + (float)diff / (float)duration;
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

	void fireWeapons(RainbowlandGame& game)
	{
		for(auto& player : game.m_players)
		{
			player.weaponTimer.updateBy(player.objectTimer.getDeltaMicros());
			auto& w = player.currentWeapon;
			if(w.ammo > 0)
			{
				if(player.isShooting && player.weaponTimer.getCurrentMicros() > w.fireDelay)
				{
					player.weaponTimer.reset();
					--w.ammo;
					auto p = Vec2::normalize(player.aim - player.transform.position)*player.collisionData.radius;
					generateBullets(game.m_bullets, w.bulletsPerShot, w.spread, player.transform.position + p, player.aim, w.damage, w.bulletPierce);
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
	}

	void generateBullets(VBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target, uint32_t damage, bool pierce)
	{

		Vec2 direction = Vec2::normalize(target - origin);
		Vec2 targetDistance = direction * 30;
		Random gen{Time::getRealTimeMicros()};
		for(uint32_t i = 0; i < count; ++i)
		{
			Vec2 t{gen.randFloat()*spread * 2 - spread, gen.randFloat()*spread * 2 - spread};
			bullets.emplace_back(Bullet{Time(), origin, origin, Vec2::normalize(targetDistance + t) * 60, 0, damage, pierce});
			bullets.back().objectTimer.reset();
		}
	}

	void moveBullets(VBullets& bullets)
	{
		for(uint32_t i = 0; i < bullets.size();)
		{
			auto displacement = bullets[i].objectTimer.getDeltaTime()*bullets[i].velocity;
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

	void updateMonsterSpawners(VMonsterSpawners& spawners, VMonsters& monsters, uint32_t playerCount)
	{
		if(playerCount == 0) return;

		Random gen{Time::getRealTimeMicros()};
		for(auto& spawner : spawners)
		{
			spawner.timer.updateBy(spawner.objectTimer.getDeltaMicros());
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
		monster.objectTimer.reset();
		monster.collisionData.set(0, 0, 0.8f);
		monster.maxVelocity = 1 + gen.randFloat() * 3.5f;
		monster.transform.position = position;
		auto scale = 1 + 0.3f - gen.randFloat()*0.6f;
		monster.transform.scale.set(scale, scale);
		monster.targetPlayer = target;
		monster.maxHealth = monster.health = gen.randInt(20, 70);
		monster.attackTimer.updateBy(Time::secondsToMicros(5));
		monster.expModifier = 1 * scale;
		monster.color.set(0, 0, 0);
		if(gen.randInt(0,99999) < 25000)
		{
			monster.expModifier *= 4;
			monster.color.set(0.2f, 0.2f, 0.7f);
		}
	}

	void moveMonsters(VMonsters& monsters, const VPlayers& players)
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
			monster.transform.position += (monster.direction*monster.maxVelocity*monster.objectTimer.getDeltaTime());
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

	void checkMonsterHurtingPlayer(RainbowlandGame& game)
	{
		for(auto& player : game.m_players)
		{
			auto pCollider = player.collisionData;
			pCollider.center = player.transform.position;
			pCollider.radius *= player.transform.scale.x;
			for(auto& monster : game.m_monsters)
			{
				auto mCollider = monster.collisionData;
				mCollider.center = monster.transform.position;
				mCollider.radius *= monster.transform.scale.x;
				if(isCircleTouchingCircle(mCollider, pCollider))
				{
					monster.attackTimer.updateBy(monster.objectTimer.getDeltaMicros());
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
					auto textSize = game.m_graphicsSystem.textHalfSize(game.m_defaultFont, game.m_perkDatabase[perk].name);
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
					game.m_guiSystem.label(name + "Label", name, game.m_defaultFont, game.m_perkDatabase[perk].name, {}, {0.5f, 0.5f}, {1, 1, 1}, TJ_Center, false);
					++perkIndex;
					column += distance;
				}
				++playerIndex;
				row -= 80;
			}
			auto textSize = game.m_graphicsSystem.textHalfSize(game.m_defaultFont, "cancel");
			Vec2 textScale{0.5f, 0.5f};
			textSize *= textScale;
			game.m_guiSystem.button("close", "perkWindow", {300 - textSize.x - 10, -300 + textSize.y + 10}, textSize, {1,1,1,1},
									Mouse::m_LeftButton, [&game]()
			{
				exitPerkMode(game);
			});
			game.m_guiSystem.label("closeLabel", "close", game.m_defaultFont, "close", {}, textScale, {1, 1, 1}, TJ_Center, false);
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
			game.m_defenseMatrixArea.center = game.m_players[0].transform.position;
			game.m_defenseMatrixArea.radius = 4;
			game.m_defenseMatrixMicros = Time::secondsToMicros(10);
			game.m_defenseMatrixTimer.reset();
		}
	}

	void updateDefenseMatrix(RainbowlandGame& game)
	{
		game.m_defenseMatrixTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
		if(game.m_defenseMatrixActive)
		{
			auto& area = game.m_defenseMatrixArea;
			//push monsters out
			for(auto& monster : game.m_monsters)
			{
				Circle monstaCollider{monster.transform.position, monster.collisionData.radius*monster.transform.scale.x};
				if(isCircleTouchingCircle(area, monstaCollider))
				{
					auto dir = Vec2::normalize(monstaCollider.center - area.center);
					monster.transform.position = area.center + dir*(area.radius + monstaCollider.radius);
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

	void activateTimeCapsule(RainbowlandGame& game)
	{
		if(!game.m_timeCapsuleActive && game.m_timeCapsuleTimer.getCurrentMicros() >= game.m_timeCapsuleMicros)
		{
			game.m_timeCapsuleActive = true;
			game.m_timeCapsuleArea.center = game.m_players[0].transform.position;
			game.m_timeCapsuleArea.radius = 10;
			game.m_timeCapsuleMicros = Time::secondsToMicros(30);
			game.m_timeCapsuleTimer.reset();
		}
	}

	void updateTimeCapsule(RainbowlandGame& game)
	{
		game.m_timeCapsuleTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
		if(game.m_timeCapsuleActive)
		{
			auto& area = game.m_timeCapsuleArea;
			std::vector<Monster*> monstersAffected;
			for(auto& monster : game.m_monsters)
			{
				Circle monstaCollider{monster.transform.position, monster.collisionData.radius*monster.transform.scale.x};
				if(isCircleTouchingCircle(area, monstaCollider))
				{
					auto distance2 = Vec2::length2(monstaCollider.center - area.center);
					auto radius2 = area.radius*area.radius;
					if(distance2 - radius2 < 0)
					{
						monstersAffected.emplace_back(&monster);
					}
				}
			}
			for(auto* monster : monstersAffected)
			{
				auto distance2 = Vec2::length2(monster->transform.position - area.center);
				auto radius2 = area.radius*area.radius;
				auto diff = radius2 - distance2;
				double scale = 1.0 - (double)diff / (double)radius2;
				clamp(0.3, 1.0, scale);
				monster->objectTimer.setTimeScale(scale);
			}
			//check if over
			if(game.m_timeCapsuleTimer.getCurrentMicros() >= game.m_timeCapsuleMicros)
			{
				game.m_timeCapsuleTimer.reset();
				game.m_timeCapsuleMicros = Time::secondsToMicros(15);
				game.m_timeCapsuleActive = false;
				for(auto* monster : monstersAffected)
				{
					monster->objectTimer.setTimeScale(Time::NORMAL_TIME);
				}
			}
		}
	}

	void generateSplatter(VKillLocations loc, RainbowlandGame& game)
	{
		Random gen(Time::getRealTimeMicros());
		for(auto p : loc)
		{
			uint32_t splatterIndex = gen.randInt(0, game.m_splatterDatabase.size() - 1);
			auto fieldHeight = 2 * std::tanf(Deg2Rad(45)) * 50;
			auto fieldWidth = 2 * fieldHeight * (game.m_window->getSizeX() / game.m_window->getSizeY());


			game.m_splatters.emplace_back(p, splatterIndex);
		}
	}
}
