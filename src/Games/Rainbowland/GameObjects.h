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
#include <Util/Timers.h>
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

	struct Player;

	typedef std::function<void(Player&, RainbowlandGame&)> UpdateLogic;
	
	enum WeaponType
	{
		Pistol,
		Shotgun,
		Uzi,
		WeaponCount
	};

	enum BonusType
	{
		IncreasedRateOfFire,
		IncreasedMovementSpeed,
		Heal,
		SlowTime,
		Weapon_Pistol,
		Weapon_Shotgun,
		Weapon_Uzi,
		BonusTypeCount
	};

	enum DirectionalMovement
	{
		Up, Down,
		Left, Right
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
		IntervalTimer attackTimer;
	};

	typedef std::vector<Monster> VMonsters;

	struct MonsterSpawner
	{
		Transform transform;
		IntervalTimer spawnTimer;
		float spawnRadius;
	};

	typedef std::vector<MonsterSpawner> VMonsterSpawners;

	typedef std::function<void()> OnAcquireLogic;

	struct Pickup
	{
		Transform transform;
		Color color;
		Circle collisionData;
		DecrementingTimer timer;
		BonusType type;
	};

	typedef std::vector<Pickup> VPickups;

	struct Bonus
	{
		BonusType type;
		std::string name;
		uint64_t duration;
		UpdateLogic acquireLogic;
		UpdateLogic updateLogic;
		UpdateLogic timeoutLogic;
	};

	typedef std::vector<Bonus> VBonuses;

	struct ActiveBonus
	{
		BonusType type;
		DecrementingTimer timer;
	};

	typedef std::vector<ActiveBonus> VActiveBonuses;

	struct RayBullet
	{
		Vec2 origin;
		Vec2 position;
		Vec2 velocity;
		float travelled;
		uint32_t damage;
	};

	typedef std::vector<Vec2> VKillLocations;
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
		VActiveBonuses bonuses;
		IntervalTimer attackTimer;
		DecrementingTimer reloadTimer;
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

	struct Perk
	{
		PerkType type;
		std::string name;
		UpdateLogic acquireLogic;
		UpdateLogic updateLogic;
	};

	typedef std::vector<Perk> VPerks;

	//waaaaaaa
	void initGame(RainbowlandGame& game);
	void cleanGame(RainbowlandGame& game);
	void initPlayer(Player& player, const VWeapons& weaponDb, const VPerks& perkDb);
	void movePlayers(const IncrementingTimer& timer, VPlayers& players, const Rect& playingField);
	void checkPlayerDeath(VPlayers& players);
	void checkLevelup(VPlayers& players, RainbowlandGame& gui);
	void grantExperience(uint32_t exp, VPlayers& players);

	void enableBonus(Player& player, BonusType bonus, RainbowlandGame& game);
	void disableBonus(Player& player, BonusType bonus, RainbowlandGame& game);
	void updateBonuses(const IncrementingTimer& timer, RainbowlandGame& game);
	void generatePickups(VKillLocations& killLocations, VPickups& pickups);
	void placePickup(VPickups& bonuses, Random& gen, Vec2 location, BonusType bonus);
	void checkPickups(VPlayers& players, RainbowlandGame& game);
	void updatePickups(const IncrementingTimer& timer, RainbowlandGame& game);

	void selectWeapon(Player& player, WeaponType weapon, const VWeapons& weaponDb);
	void fireWeapon(const IncrementingTimer& timer, Player& player, VRayBullets& bullets, const GraphicsSystem& graphicsSystem, const Camera& camera);
	void generateBullets(VRayBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target, uint32_t damage);
	void moveBullets(const IncrementingTimer& timer, VRayBullets& bullets);
	void checkBulletHits(VRayBullets& bullets, VMonsters& monsters);

	void updateMonsterSpawners(const IncrementingTimer& timer, VMonsterSpawners& spawners, VMonsters& monsters, uint32_t playerCount);
	void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target);
	void moveMonsters(const IncrementingTimer& timer, VMonsters& monsters, const VPlayers& players);
	void checkMonsterHurtingPlayer(const IncrementingTimer& timer, VMonsters& monsters, VPlayers& players);
	void killMonsters(VMonsters& monsters, VKillLocations& killLocations, VPlayers& players);

	bool enterPerkMode(RainbowlandGame& game);
	void exitPerkMode(RainbowlandGame& game);
	void generatePerks(VPlayers& players, const VPerks& perkDb);
	bool allPlayersChosePerk(VPlayers& players);
	void applyPerksForPlayers(RainbowlandGame& game);
	void updatePerks(RainbowlandGame& game);
}
