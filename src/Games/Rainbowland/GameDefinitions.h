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
	
   struct SessionPreparationData
   {
      int32_t classes[4];
      int32_t controllers[5];
      bool start;
   };




	enum BonusType
	{
		AtomicBomb,
		SlowTime,
		Heal,
		MassHeal,
		IncreasedMovementSpeed,
		IncreasedRateOfFire,
		BonusTypeCount
	};

   enum BonusIcon
   {
      AtomicBombIcon = 1,
      SlowTimeIcon = 2,
      TwoThousandExpIcon = 3,
      ExpTimes5Icon = 4,
      IncreasedMovementSpeedIcon = 5,
      ExpTimes2Icon = 7,
      FreezeIcon = 11,
      HealIcon = 14,
      MassHealIcon = 15,
      ShieldIcon = 16,
      MassShieldIcon = 17,
      ExpTimes3Icon = 18,
      FireBulletsIcon = 22,
      MassFireBulletsIcon = 23,
      RingOfFireIcon = 24,
      IncreasedRateOfFireIcon = 25,
      MassIncreasedRateOfFireIcon = 26,
      OneThousandExpIcon = 27,
   };

	enum WeaponType
	{
		Pistol,
		Shotgun,
		Uzi,
		Sniper,
      RPG,
		WeaponTypeCount
	};

   enum WeaponIcon
   {
      MinigunIcon = 8,
      ShotgunIcon = 9,
      RPGIcon = 10,
      LargeMachineGunIcon = 19,
      RevolverIcon = 20,
      M16Icon = 21,
      P90Icon = 28,
      HedgehogIcon = 29,
      PistolIcon = 30,
      UziIcon = 31,
      SniperIcon = 32
   };

   enum MonsterType
   {
      Flower,
      Ladybug,
      Butterfly,
      MonsterTypeCount
   };

	enum DirectionalMovement
	{
		Up, Down,
		Left, Right
	};

   enum SteeringBehaviour
   {
      SB_Seek,
      SB_Flee,
      SB_Arrive,
      SB_Pursuit,
      SB_Wander
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

   enum BrainState
   {
      BS_Wander,
      BS_Flank,
      BS_Attack
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
		Circle collisionData_attack;
      Circle collisionData_hitbox;
      Circle collisionData_separation;
		Vec2 direction;
		float maxSpeed;
		float turnSpeed;
		int32_t maxHealth;
		int32_t health;
		Time attackTimer;
      uint64_t attackDelay;
      uint32_t damage;
		uint32_t expGain;
      MonsterType type;
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
      uint32_t iconIndex;
	};

	typedef std::vector<Pickup> VPickups;

	typedef std::function<void(Vec2, Player&, RainbowlandGame&)> BonusAcquireLogic;
	typedef std::function<void(RainbowlandGame&)> BonusTimeoutLogic;

	struct Bonus
	{
		BonusType type;
		std::string name;
		uint64_t durationMicros;
		BonusAcquireLogic acquireLogic;
		BonusTimeoutLogic timeoutLogic;
	};

	typedef std::vector<Bonus> VBonuses;

	struct ActiveBonus
	{
		Time timer;
		uint64_t duration;
		BonusType type;
	};

	typedef std::vector<ActiveBonus> VActiveBonuses;

	struct BloodSplatter
	{
		Transform transform;
		Color color;
		uint32_t splatterImage;
	};

	typedef std::vector<BloodSplatter> VSplatters;


   struct KillLocation
   {
      Transform transform;
      MonsterType monster;
   };
	typedef std::vector<KillLocation> VKillLocations;

	struct Bullet
	{
		Time objectTimer;
		Vec2 trail;
		Vec2 position;
		Vec2 oldPosition;
		Vec2 velocity;
		float travelled;
		uint32_t damage;
      void* owner;
		bool pierce;
		bool dead;
	};

	typedef std::vector<Bullet> VBullets;

	struct Blast
	{
		Time objectTimer;
		uint64_t duration;
		Circle area;
		float maxRadius;
		int32_t damage;
      float rotation;
      void* owner;
	};

	typedef std::vector<Blast> VBlasts;

   struct Rocket
   {
      Time objectTimer;
      uint64_t duration;
      Circle body;
      Vec2 direction;
      float speed;
      uint32_t damage;
      void* owner;
   };

   typedef std::vector<Rocket> VRockets;

	struct DefenseMatrix
	{
		Time timer;
		uint64_t updateDelay;
      float cooldownSeconds;
      float durationSeconds;
		Circle area;
		bool active;
	};

	struct TimeCapsule
	{
		Time timer;
		uint64_t updateDelay;
      float cooldownSeconds;
      float durationSeconds;
		Circle area;
		bool active;
	};

	struct Blink
	{
		Time timer;
		uint64_t updateDelay;
      float cooldownSeconds;
      float durationSeconds;
		Circle area;
		Vec2 target;
		bool active;
	};

   struct Turret
   {
      Time timer;
      uint64_t updateDelay;
      float cooldownSeconds;
      float durationSeconds;
      Circle area;
      Weapon weapon;
      Time weaponTimer;
      Vec2 aim;
      uint32_t killCount;
      bool active;
   };

	enum PerkType
	{
		Regeneration,
      GreaterRegeneration,
		PoisonBullets,
		Radioactive,
		AmmoManiac,
      InstantWinner,
		Fastloader,
		ReflexBoosted,
      Dodger,
      Ninja,
      Assassin,
      HealthBoost,
      EquipedWeaponDamageBoost,
      EquipedWeaponAmmoBoost,
      FasterCooldown,
      HotTempered,
      AngryReloader,
      StationaryReloader,
      FastShot,
      ExtraBullet,
      PerkExpert,
      PerkMaster,
      MonsterCleaner,
		PerkTypeCount
	};

   typedef std::function<bool(Player&)> PlayerCheck;
   struct Perk
   {
      PerkType type;
      std::string name;
      std::string description;
      PlayerCheck dependencyCheck;
      UpdateLogic acquireLogic;
      UpdateLogic updateLogic;
      bool repeatable;
   };

   typedef std::vector<Perk> VPerks;

	struct Player
	{
		Time objectTimer;
		Time weaponTimer;
		Transform transform;
		Color color;
		Circle collisionData;
		Vec2 direction;
      Vec2 targetDirection;
      bool directionActive[4];
      float currentSpeed;
		float maxSpeed;
		float acceleration;
		Vec2 aim;
      Vec2 aimDirection;
      float aimDistance;
      float maxAimDistance;
      float minAimDistance;
		Weapon currentWeapon;
		float rateOfFireMultiplier;
		float reloadMultiplier;
		float ammoMultiplier;
      uint32_t extraBullets;
      uint32_t dodgeChance;
		uint32_t bonusDamage;
		uint32_t maxHealth;
		int32_t health;
		uint32_t regenDelayForOneHealth;
		uint32_t perksPerLevel;
      uint32_t selectedPerkIndex;
		uint32_t id;
      uint32_t killCount;
      float* skillCooldown;
		std::vector<PerkType> availablePerks;
		std::vector<PerkType> acquiredPerks;
		std::vector<PerkType> selectablePerks;
		std::function<void(Player&, RainbowlandGame&)> ability;
		PerkType chosenPerk;
		bool isShooting;
      bool isAimRelative;
	};

	typedef std::vector<Player> VPlayers;


	//waaaaaaa
	void gameplay_logic(RainbowlandGame& game);
	void gameplay_render(RainbowlandGame& game);

	void initGame(RainbowlandGame& game);
   void initSession(RainbowlandGame& game);
	void cleanSession(RainbowlandGame& game);
	void initPlayer(Player& player, uint32_t id, RainbowlandGame& game);
	void movePlayers(RainbowlandGame& game);
	void orientPlayers(VPlayers& players);
   void updatePlayerAim(VPlayers& players);
	void checkPlayerDeath(RainbowlandGame& game);
	void checkLevelup(RainbowlandGame& game);
	void grantExperience(uint32_t exp, RainbowlandGame& game);
	void fixupCamera(RainbowlandGame& game);
   void updateDifficulty(RainbowlandGame& game);

	void enableBonus(BonusType bonus, Vec2 pickupPosition, Player& player, RainbowlandGame& game);
	void disableBonus(Player& player, BonusType bonus, RainbowlandGame& game);
	void updateBonuses(RainbowlandGame& game);
	void generatePickups(VKillLocations& killLocations, RainbowlandGame& game);
	void placePickup(RainbowlandGame& game, Vec2 location, BonusType bonus, WeaponType weapon);
	void checkPickups(RainbowlandGame& game);
	void updatePickups(RainbowlandGame& game);

	void selectWeapon(Player& player, WeaponType weapon, const VWeapons& weaponDb);
	void calculateWeaponBonuses(Player& player, const VWeapons& weaponDb);
	void fireWeapons(RainbowlandGame& game);
	void generateBullets(VBullets& bullets, Random& gen, uint32_t count, float spread, Vec2 origin, Vec2 direction, uint32_t damage, bool pierce, void* owner);
	void generateBlast(VBlasts& blasts, Vec2 location, float startRadius, float endRadius, float duration, int32_t damage, void* owner);
   void generateRocket(VRockets& rockets, Vec2 location, Vec2 direction, uint32_t damage, void* owner);
   void moveBullets(VBullets& bullets);
	void moveBlasts(VBlasts& blasts);
   void moveRockets(VRockets& rockets);
   void updateBullets(RainbowlandGame& game);
   void updateBlasts(RainbowlandGame& game);
   void updateRockets(RainbowlandGame& game);
   void increaseKillCount(RainbowlandGame& game, void* owner);

	void updateMonsterSpawners(RainbowlandGame& game);
   void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target, RainbowlandGame& game);
	void runMonsterAI(RainbowlandGame& game);
	void moveMonsters(RainbowlandGame& game);
	void orientMonsters(VMonsters& monsters);
   void hurtMonster(Monster& monster, uint32_t amount);
	void checkMonsterHurtingPlayer(RainbowlandGame& game);
	void killMonsters(RainbowlandGame& game, VKillLocations& killLocations, bool grantExp);

	void enterPerkMode(RainbowlandGame& game);
	void exitPerkMode(RainbowlandGame& game);
	void generatePerks(VPlayers& players, const VPerks& perkDb);
	bool allPlayersChosePerk(VPlayers& players);
	void applyPerksForPlayers(RainbowlandGame& game);
	void updatePerks(RainbowlandGame& game);

	void activateDefenseMatrix(Player& player, RainbowlandGame& game);
	void activateTimeCapsule(Player& player, RainbowlandGame& game);
	void activateBlink(Player& player, RainbowlandGame& game);
   void activateTurret(Player& player, RainbowlandGame& game);
	
	void updateDefenseMatrix(RainbowlandGame& game);
	void updateTimeCapsule(RainbowlandGame& game);
	void updateBlink(RainbowlandGame& game);
   void updateTurret(RainbowlandGame& game);

	void generateSplatter(VKillLocations loc, RainbowlandGame& game);

	Vec2 steer_seek(Vec2 position, float maxSpeed, Vec2 targetLocation);
	Vec2 steer_flee(Vec2 position, float maxSpeed, Vec2 targetLocation);
	Vec2 steer_arrive(Vec2 position, float maxSpeed, Vec2 targetLocation);
	Vec2 steer_pursuit(Vec2 position, Vec2 heading, float maxSpeed, Vec2 targetPosition, Vec2 targetHeading, float targetMaxSpeed);
}
