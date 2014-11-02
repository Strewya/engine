#pragma once
/********************************************
*	class:	GameObjects
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Color.h>
#include <Util/Rect.h>
#include <Util/Time.h>
#include <Util/Transform.h>
/******* end header inclusion *******/

namespace Core
{
	class Camera;
	class GraphicsSystem;
	class Time;

	enum Weapon
	{
		Pistol,
		Shotgun,
		Uzi,
		Atombomb
	};


	struct Player
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		Vec2 velocity;
		Vec2 maxVelocity;
		Vec2 acceleration;
		Vec2 direction;
		Time weaponTimer;
		Time bonusTimer;
		uint64_t weaponDelay;
		uint32_t rateOfFireMultiplier;
		uint64_t bonusDuration;
		Vec2 aim;
		Weapon currentWeapon;
		bool isShooting;
		bool hasBonus;
	};

	typedef std::vector<Player> VPlayers;

	void initPlayer(Player& player);
	void movePlayers(const Time& timer, VPlayers& players, uint32_t activePlayerCount, const Rect& playingField);
	void selectWeapon(Player& player, Weapon weapon);

	struct Monster
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		Vec2 direction;
		float maxVelocity;
		uint32_t targetPlayer;
	};

	typedef std::vector<Monster> VMonsters;

	void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target);
	void moveMonsters(const Time& timer, VMonsters& monsters, const VPlayers& players);

	struct MonsterSpawner
	{
		Transform transform;
		Time timer;
		uint64_t spawnCooldown;
		float spawnRadius;
	};

	typedef std::vector<MonsterSpawner> VMonsterSpawners;

	void updateMonsterSpawners(const Time& timer, VMonsterSpawners& spawners, VMonsters& monsters, uint32_t playerCount);

	struct Bonus
	{
		Transform transform;
		Color color;
		Rect boundingBox;
	};

	typedef std::vector<Bonus> VBonuses;
	typedef std::vector<Vec2> VKillLocations;

	void generateBonuses(VKillLocations& killLocations, VBonuses& bonuses);
	void checkBonusPickup(VPlayers& players, VBonuses& bonuses);
	void updateBonuses(const Time& timer, VPlayers& players);

	struct RayBullet
	{
		Vec2 origin;
		Vec2 position;
		Vec2 velocity;
		float travelled;
	};

	typedef std::vector<RayBullet> VRayBullets;

	void generateBullets(VRayBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target);
	void moveBullets(const Time& timer, VRayBullets& bullets);
	void killMonsters(VRayBullets& bullets, VMonsters& monsters, VKillLocations& killLocations);
	void fireWeapon(Player& player, VRayBullets& bullets, const GraphicsSystem& graphicsSystem, const Camera& camera);
}
