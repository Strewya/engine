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
		Sniper,
		WeaponTypeCount
	};

	enum BonusType
	{
		IncreasedRateOfFire,
		IncreasedMovementSpeed,
		Heal,
		SlowTime,
		BonusTypeCount
	};

	enum DirectionalMovement
	{
		Up, Down,
		Left, Right
	};

	struct Player;

	typedef std::function<void(Player&, RainbowlandGame&)> UpdateLogic;

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
		bool bulletPierce;
	};

	typedef std::vector<Weapon> VWeapons;

	enum SteeringBehaviour
	{
		SB_Seek,
		SB_Flee,
		SB_Arrive,
		SB_Pursuit
	};

	struct Brain
	{
		Time timer;
		uint64_t updateDelay;
		uint32_t targetPlayer;
		Vec2 targetLocation;
		SteeringBehaviour steerLogic;
	};

	struct Monster
	{
		Brain brain;
		Time objectTimer;
		Transform transform;
		Color color;
		Circle collisionData;
		Vec2 direction;
		float maxSpeed;
		float turnSpeed;
		int32_t maxHealth;
		int32_t health;
		Time attackTimer;
		float expModifier;
	};

	typedef std::vector<Monster> VMonsters;

	struct MonsterSpawner
	{
		Time objectTimer;
		Transform transform;
		Time timer;
		uint64_t spawnCooldown;
		float spawnRadius;
	};

	typedef std::vector<MonsterSpawner> VMonsterSpawners;

	struct Pickup
	{
		Time objectTimer;
		Transform transform;
		Color color;
		Circle collisionData;
		uint64_t duration;
		BonusType bonus;
		WeaponType weapon;
	};

	typedef std::vector<Pickup> VPickups;

	struct BloodSplatter
	{
		Transform transform;
		Color color;
		uint32_t splatterImage;
	};

	typedef std::vector<BloodSplatter> VSplatters;

	struct Bonus
	{
		BonusType type;
		std::string name;
		uint64_t durationMicros;
		UpdateLogic acquireLogic;
		UpdateLogic timeoutLogic;
	};

	typedef std::vector<Bonus> VBonuses;

	struct ActiveBonus
	{
		Time timer;
		uint64_t duration;
		BonusType type;
	};

	typedef std::vector<ActiveBonus> VActiveBonuses;
	typedef std::vector<Vec2> VKillLocations;

	struct Bullet
	{
		Time objectTimer;
		Vec2 trail;
		Vec2 position;
		Vec2 oldPosition;
		Vec2 velocity;
		float travelled;
		uint32_t damage;
		bool pierce;
		bool dead;
	};

	typedef std::vector<Bullet> VBullets;

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
		Time objectTimer;
		Time weaponTimer;
		Transform transform;
		Color color;
		Circle collisionData;
		Vec2 direction;
		float maxSpeed;
		float acceleration;
		bool directions[4];
		Vec2 aim;
		VActiveBonuses bonuses;
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
		uint32_t id;
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
	void initPlayer(Player& player, uint32_t id, RainbowlandGame& game);
	void movePlayers(RainbowlandGame& game);
	void orientPlayers(VPlayers& players);
	void checkPlayerDeath(RainbowlandGame& game);
	void checkLevelup(RainbowlandGame& game);
	void grantExperience(uint32_t exp, VPlayers& players);
	void updateGuiLabels(RainbowlandGame& game);
	void fixupCamera(RainbowlandGame& game);

	void enableBonus(Player& player, Bonus& bonus, RainbowlandGame& game);
	void disableBonus(Player& player, BonusType bonus, RainbowlandGame& game);
	void updateBonuses(RainbowlandGame& game);
	void generatePickups(VKillLocations& killLocations, RainbowlandGame& game);
	void placePickup(VPickups& pickups, Vec2 location, BonusType bonus, WeaponType weapon);
	void checkPickups(RainbowlandGame& game);
	void updatePickups(RainbowlandGame& game);

	void selectWeapon(Player& player, WeaponType weapon, const VWeapons& weaponDb);
	void calculateWeaponBonuses(Player& player, const VWeapons& weaponDb);
	void fireWeapons(RainbowlandGame& game);
	void generateBullets(VBullets& bullets, Random& gen, uint32_t count, float spread, const Vec2& origin, const Vec2& target, uint32_t damage, bool pierce);
	void moveBullets(VBullets& bullets);
	void findBulletHits(const Bullet& bullet, const VMonsters& monsters, std::vector<Monster*>& outMonsters);
	void updateBullets(VBullets& bullets, VMonsters& monsters);

	void updateMonsterSpawners(RainbowlandGame& game);
	void generateMonster(VMonsters& monsters, Random& gen, Vec2 position, uint32_t target);
	void runMonsterAI(RainbowlandGame& game);
	void moveMonsters(RainbowlandGame& game);
	void orientMonsters(VMonsters& monsters);
	void hurtMonster(Monster& monster, uint32_t amount);
	void checkMonsterHurtingPlayer(RainbowlandGame& game);
	void killMonsters(RainbowlandGame& game, VKillLocations& killLocations);

	bool enterPerkMode(RainbowlandGame& game);
	void exitPerkMode(RainbowlandGame& game);
	void generatePerks(VPlayers& players, const VPerks& perkDb);
	bool allPlayersChosePerk(VPlayers& players);
	void applyPerksForPlayers(RainbowlandGame& game);
	void updatePerks(RainbowlandGame& game);

	void activateDefenseMatrix(RainbowlandGame& game);
	void updateDefenseMatrix(RainbowlandGame& game);

	void activateTimeCapsule(RainbowlandGame& game);
	void updateTimeCapsule(RainbowlandGame& game);
	
	void activateBlink(RainbowlandGame& game);

	void generateSplatter(VKillLocations loc, RainbowlandGame& game);

	Vec2 steer_seek(Vec2 position, float maxSpeed, Vec2 targetLocation);
	Vec2 steer_flee(Vec2 position, float maxSpeed, Vec2 targetLocation);
	Vec2 steer_arrive(Vec2 position, float maxSpeed, Vec2 targetLocation);
	Vec2 steer_pursuit(Vec2 position, Vec2 heading, float maxSpeed, Vec2 targetPosition, Vec2 targetHeading, float targetMaxSpeed);
}
