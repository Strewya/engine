#pragma once
/********************************************
*	class:	GameObjects
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <functional>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Circle.h>
#include <Util/Color.h>
#include <Util/Time.h>
#include <Util/Transform.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class Camera;
	class RainbowlandGame;
	class Random;
	class Rect;
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
		SlowTime,
		WeaponDrop_Pistol,
		WeaponDrop_Shotgun,
		WeaponDrop_Uzi,
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
		Circle collisionData;
		Vec2 direction;
		float maxVelocity;
		uint32_t targetPlayer;
		int32_t maxHealth;
		int32_t health;
		Time attackTimer;
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
		Circle collisionData;
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
		InstantWinner,
		Fastloader,
		ReflexBoosted,
		PerkTypeCount
	};

	struct Player
	{
		Transform transform;
		Color color;
		Circle collisionData;
		Vec2 velocity;
		Vec2 maxVelocity;
		Vec2 acceleration;
		bool directions[4];
		Vec2 aim;
		std::vector<BonusEffect> bonuses;
		Time weaponTimer;
		Weapon currentWeapon;
		uint32_t bonusDamage;
		float rateOfFireMultiplier;
		float reloadMultiplier;
		float ammoMultiplier;
		uint32_t maxHealth;
		int32_t health;
		uint32_t regenDelayForOneHealth;
		uint32_t experience;
		uint32_t experienceForNextLevel;
		uint32_t level;
		uint32_t perkPoints;
		uint32_t perksPerLevel;
		std::vector<PerkType> availablePerks;
		std::vector<PerkType> acquiredPerks;
		std::vector<PerkType> selectablePerks;
		PerkType chosenPerk;
		bool isShooting;
	};

	typedef std::vector<Player> VPlayers;

	typedef std::function<void(Player&, RainbowlandGame&)> PerkUpdateLogic;
	
	struct Perk
	{
		PerkType type;
		std::string name;
		PerkUpdateLogic acquireLogic;
		PerkUpdateLogic updateLogic;
	};

	typedef std::vector<Perk> VPerks;

	//waaaaaaa
	void initGame(RainbowlandGame& game);
	void cleanGame(RainbowlandGame& game);
	void initPlayer(Player& player, const VWeapons& weaponDb, const VPerks& perkDb);
	void movePlayers(const Time& timer, VPlayers& players, const Rect& playingField);
	void checkPlayerDeath(VPlayers& players);
	void checkLevelup(VPlayers& players, RainbowlandGame& gui);
	void grantExperience(uint32_t exp, VPlayers& players);

	void enableEffect(Player& player, Bonus& bonus, RainbowlandGame& game);
	void disableEffect(Player& player, EffectType effect, RainbowlandGame& game);
	void updateBonusEffects(const Time& timer, RainbowlandGame& game);
	void generateBonuses(VKillLocations& killLocations, VBonuses& bonuses);
	void placeBonus(VBonuses& bonuses, Random& gen, Vec2 location, EffectType effect);
	void checkBonusPickup(VPlayers& players, RainbowlandGame& game);
	void updateBonuses(const Time& timer, RainbowlandGame& game);

	void selectWeapon(Player& player, WeaponType weapon, const VWeapons& weaponDb);
	void fireWeapon(const Time& timer, Player& player, VRayBullets& bullets, const GraphicsSystem& graphicsSystem, const Camera& camera);
	void generateBullets(VRayBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target, uint32_t damage);
	void moveBullets(const Time& timer, VRayBullets& bullets);
	void checkBulletHits(VRayBullets& bullets, VMonsters& monsters);

	void updateMonsterSpawners(const Time& timer, VMonsterSpawners& spawners, VMonsters& monsters, uint32_t playerCount);
	void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target);
	void moveMonsters(const Time& timer, VMonsters& monsters, const VPlayers& players);
	void checkMonsterHurtingPlayer(const Time& timer, VMonsters& monsters, VPlayers& players);
	void killMonsters(VMonsters& monsters, VKillLocations& killLocations, VPlayers& players);

	bool enterPerkMode(RainbowlandGame& game);
	void exitPerkMode(RainbowlandGame& game);
	void generatePerks(VPlayers& players, const VPerks& perkDb);
	bool allPlayersChosePerk(VPlayers& players);
	void applyPerksForPlayers(RainbowlandGame& game);
	void updatePerks(RainbowlandGame& game);
}
