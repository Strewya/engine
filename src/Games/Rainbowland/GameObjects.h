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
	
	enum WeaponType
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
		WeaponDrop,
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

	struct Weapon
	{
		WeaponType type;
		std::string name;
		uint64_t fireDelay;
		uint64_t reloadDelay;
		uint32_t damage;
		uint32_t ammo;
		uint32_t maxAmmo;
		uint32_t bulletsPerShot;
		float spread;
	};

	typedef std::vector<Weapon> VWeapons;

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

	struct MonsterSpawner
	{
		Transform transform;
		Time timer;
		uint64_t spawnCooldown;
		float spawnRadius;
	};

	typedef std::vector<MonsterSpawner> VMonsterSpawners;

	struct Bonus
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		EffectType effect;
		WeaponType weapon;
		Time timer;
		uint64_t duration;
	};

	typedef std::vector<Bonus> VBonuses;
	typedef std::vector<Vec2> VKillLocations;

	struct RayBullet
	{
		Vec2 origin;
		Vec2 position;
		Vec2 velocity;
		float travelled;
		uint32_t damage;
	};

	typedef std::vector<RayBullet> VRayBullets;

	enum PerkType
	{
		Regeneration,
		PoisonBullets,
		Radioactive,
		AmmoManiac,
		Greaser,
		Fastloader,
		MeanLeanExpMachine,
		HeavyRunner,
		Dodger,
		HotTempered,
		StationaryReloader,
		PerkTypeCount
	};

	struct Perk
	{
		PerkType type;
		std::string name;
		Rect button;
	};

	typedef std::vector<Perk> VPerks;

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
		std::vector<BonusEffect> bonuses;
		Time weaponTimer;
		Weapon currentWeapon;
		uint32_t bonusDamage;
		uint32_t rateOfFireMultiplier;
		uint32_t movementSpeedMultiplier;
		uint32_t maxHealth;
		int32_t health;
		Time regenTimer;
		uint32_t regeneration;
		uint32_t experience;
		uint32_t experienceForNextLevel;
		uint32_t level;
		uint32_t perkPoints;
		bool isShooting;
	};

	typedef std::vector<Player> VPlayers;

	//waaaaaaa
	void initGame(RainbowlandGame& game);
	void cleanGame(RainbowlandGame& game);
	void initPlayer(Player& player, const VWeapons& weaponDb);
	void movePlayers(const Time& timer, VPlayers& players, const Rect& playingField);
	void checkPlayerDeath(VPlayers& players);
	void checkLevelup(VPlayers& players);

	void enableEffect(Player& player, Bonus& bonus, const VWeapons& weaponDb);
	void disableEffect(Player& player, EffectType effect);
	void updateBonusEffects(const Time& timer, VPlayers& players);
	void generateBonuses(VKillLocations& killLocations, VBonuses& bonuses);
	void checkBonusPickup(VPlayers& players, VBonuses& bonuses, const VWeapons& weaponDb);
	void updateBonuses(const Time& timer, VBonuses& bonuses);

	void selectWeapon(Player& player, WeaponType weapon, const VWeapons& weaponDb);
	void fireWeapon(const Time& timer, Player& player, VRayBullets& bullets, const GraphicsSystem& graphicsSystem, const Camera& camera);
	void generateBullets(VRayBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target, uint32_t damage);
	void moveBullets(const Time& timer, VRayBullets& bullets);
	void killMonsters(VRayBullets& bullets, VMonsters& monsters, VKillLocations& killLocations, VPlayers& players);

	void updateMonsterSpawners(const Time& timer, VMonsterSpawners& spawners, VMonsters& monsters, uint32_t playerCount);
	void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target);
	void moveMonsters(const Time& timer, VMonsters& monsters, const VPlayers& players);
	void checkMonsterHurtingPlayer(VMonsters& monsters, VPlayers& players);

	bool enterPerkMode(RainbowlandGame& game);
	void exitPerkMode(RainbowlandGame& game);
	void generatePerks(VPerks& perks, const VPerks& perkDb);
	void mouseClickPerkMode(RainbowlandGame& game, Vec2 clickPos);
	void applyPerk(Player& player, Perk& perk);
	void drawPerkModeGui(RainbowlandGame& game);
}
