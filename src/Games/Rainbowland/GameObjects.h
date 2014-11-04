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
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class Camera;
	class RainbowlandGame;
	class GraphicsSystem;
	
	enum Weapon
	{
		Pistol,
		Shotgun,
		Uzi,
		WeaponCount
	};

	enum EffectType
	{
		IncreasedRateOfFire,
		IncreasedMovementSpeed,
		Heal,
		EffectTypeCount
	};

	enum DirectionalMovement
	{
		Up, Down,
		Left, Right
	};

	struct BonusEffect
	{
		Time timer;
		uint64_t duration;
		EffectType type;
	};

	struct Player
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		Vec2 velocity;
		Vec2 maxVelocity;
		Vec2 acceleration;
		bool directions[4];
		Vec2 aim;
		Time weaponTimer;
		std::vector<BonusEffect> bonuses;
		uint64_t weaponDelay;
		uint32_t rateOfFireMultiplier;
		uint32_t movementSpeedMultiplier;
		uint32_t maxHealth;
		int32_t health;
		uint32_t regeneration;
		Weapon currentWeapon;
		bool isShooting;
	};

	typedef std::vector<Player> VPlayers;

	void initPlayer(Player& player);
	void movePlayers(const Time& timer, VPlayers& players, const Rect& playingField);
	void selectWeapon(Player& player, Weapon weapon);
	void enableEffect(Player& player, EffectType effect);
	void disableEffect(Player& player, EffectType effect);
	void updateBonusEffects(const Time& timer, VPlayers& players);
	void checkPlayerDeath(VPlayers& players);

	struct Monster
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		Vec2 direction;
		float maxVelocity;
		uint32_t targetPlayer;
		int32_t maxHealth;
		int32_t health;
	};

	typedef std::vector<Monster> VMonsters;

	void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target);
	void moveMonsters(const Time& timer, VMonsters& monsters, const VPlayers& players);
	void checkMonsterHurtingPlayer(VMonsters& monsters, VPlayers& players);

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
		EffectType effect;
	};

	typedef std::vector<Bonus> VBonuses;
	typedef std::vector<Vec2> VKillLocations;

	void generateBonuses(VKillLocations& killLocations, VBonuses& bonuses);
	void checkBonusPickup(VPlayers& players, VBonuses& bonuses);

	struct RayBullet
	{
		Vec2 origin;
		Vec2 position;
		Vec2 velocity;
		float travelled;
		uint32_t damage;
	};

	typedef std::vector<RayBullet> VRayBullets;

	void generateBullets(VRayBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target, uint32_t damage);
	void moveBullets(const Time& timer, VRayBullets& bullets);
	void killMonsters(VRayBullets& bullets, VMonsters& monsters, VKillLocations& killLocations);
	void fireWeapon(Player& player, VRayBullets& bullets, const GraphicsSystem& graphicsSystem, const Camera& camera);


	//waaaaaaa
	void initGame(RainbowlandGame& game);
	void cleanGame(RainbowlandGame& game);
}
