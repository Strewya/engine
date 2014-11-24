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
		Weapon_Sniper,
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

	struct Monster
	{
		Time objectTimer;
		Transform transform;
		Color color;
		Circle collisionData;
		Vec2 direction;
		float maxVelocity;
		uint32_t targetPlayer;
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
	};

	typedef std::vector<Pickup> VPickups;

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
		Vec2 origin;
		Vec2 position;
		Vec2 velocity;
		float travelled;
		uint32_t damage;
		bool pierce;
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
		Vec2 velocity;
		Vec2 maxVelocity;
		Vec2 acceleration;
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
	void movePlayers(VPlayers& players, const Rect& playingField);
	void orientPlayers(VPlayers& players);
	void checkPlayerDeath(RainbowlandGame& game);
	void checkLevelup(RainbowlandGame& game);
	void grantExperience(uint32_t exp, VPlayers& players);
	void updateGuiLabels(RainbowlandGame& game);
	void fixupCamera(VPlayers& players, Camera& camera);

	void enableBonus(Player& player, Bonus& bonus, RainbowlandGame& game);
	void disableBonus(Player& player, BonusType bonus, RainbowlandGame& game);
	void updateBonuses(RainbowlandGame& game);
	void generatePickups(VKillLocations& killLocations, VPickups& pickups, const VBonuses& bonusDb);
	void placePickup(VPickups& pickups, Random& gen, Vec2 location, BonusType bonus);
	void checkPickups(RainbowlandGame& game);
	void updatePickups(RainbowlandGame& game);

	void selectWeapon(Player& player, WeaponType weapon, const VWeapons& weaponDb);
	void calculateWeaponBonuses(Player& player, const VWeapons& weaponDb);
	void fireWeapons(RainbowlandGame& game);
	void generateBullets(VBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target, uint32_t damage, bool pierce);
	void moveBullets(VBullets& bullets);
	void findBulletHits(const Bullet& bullet, const VMonsters& monsters, std::vector<Monster*>& outMonsters);
	void updateBullets(VBullets& bullets, VMonsters& monsters);

	void updateMonsterSpawners(VMonsterSpawners& spawners, VMonsters& monsters, uint32_t playerCount);
	void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target);
	void moveMonsters(VMonsters& monsters, const VPlayers& players);
	void orientMonsters(VMonsters& monsters);
	void hurtMonster(Monster& monster, uint32_t amount);
	void checkMonsterHurtingPlayer(RainbowlandGame& game);
	void killMonsters(VMonsters& monsters, VKillLocations& killLocations, VPlayers& players);

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
}
