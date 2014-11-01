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
	class Time;

	struct Player
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		Vec2 velocity;
		Vec2 maxVelocity;
		Vec2 acceleration;
		Vec2 direction;
	};

	typedef std::vector<Player> VPlayers;

	void movePlayers(const Time& timer, VPlayers& players, uint32_t activePlayerCount, const Rect& playingField);

	struct Monster
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		Vec2 velocity;
		Vec2 maxVelocity;
		Vec2 direction;
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
		uint32_t m_imageID;
	};

	typedef std::vector<Bonus> VBonuses;

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
	void killMonsters(VRayBullets& bullets, VMonsters& monsters);
}
