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
#include <Games/Rainbowland/Abilities.h>
#include <Games/Rainbowland/Bonus.h>
#include <Games/Rainbowland/Grid.h>
#include <Games/Rainbowland/Monster.h>
#include <Games/Rainbowland/Perk.h>
#include <Games/Rainbowland/Weapon.h>
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

   enum DirectionalMovement
   {
      Up, Down,
      Left, Right
   };

   struct Pickup
   {
      CooldownTimer objectTimer;
      Transform transform;
      Color color;
      Circle collisionData;
      BonusType bonus;
      WeaponType weapon;
      uint32_t iconIndex;
   };

   typedef std::vector<Pickup> VPickups;

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
      Timer objectTimer;
      Vec2f trail;
      Vec2f position;
      Vec2f oldPosition;
      Vec2f velocity;
      float travelled;
      uint32_t damage;
      void* owner;
      bool pierce;
      bool dead;
   };

   typedef std::vector<Bullet> VBullets;

   struct Blast
   {
      CooldownTimer objectTimer;
      Circle area;
      float maxRadius;
      int32_t damage;
      float rotation;
      void* owner;
   };

   typedef std::vector<Blast> VBlasts;

   struct Rocket
   {
      CooldownTimer objectTimer;
      Circle body;
      Vec2f direction;
      float speed;
      uint32_t damage;
      void* owner;
   };

   typedef std::vector<Rocket> VRockets;

   struct Player
   {
      //misc
      Timer objectTimer;
      uint32_t killCount;
      uint32_t id;
      //rendering
      Transform transform;
      Color color;
      //collision
      Circle collisionData;
      //movement
      Vec2f direction;
      Vec2f targetDirection;
      bool directionActive[4];
      float currentSpeed;
      float maxSpeed;
      float acceleration;
      //weapon related
      CooldownTimer weaponTimer;
      Vec2f aim;
      Vec2f aimDirection;
      bool isAimRelative;
      float aimDistance;
      float maxAimDistance;
      float minAimDistance;
      Weapon currentWeapon;
      float rateOfFireMultiplier;
      float reloadMultiplier;
      float ammoMultiplier;
      uint32_t extraBullets;
      uint32_t bonusDamage;
      bool isShooting;
      //collision response
      uint32_t dodgeChance;
      uint32_t maxHealth;
      int32_t health;
      //perk related
      uint32_t regenDelayForOneHealth;
      //levelup
      uint32_t perksPerLevel;
      uint32_t selectedPerkIndex;
      VActivePerks acquiredPerks;
      std::vector<PerkType> availablePerks;
      std::vector<PerkType> selectablePerks;
      PerkType chosenPerk;
      //skill
      float skillCooldown;
      AbilityType ability;
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

   void enableBonus(BonusType bonus, Vec2f pickupPosition, Player& player, RainbowlandGame& game);
   void disableBonus(BonusType bonus, RainbowlandGame& game);
   void updateBonuses(RainbowlandGame& game);
   void generatePickups(VKillLocations& killLocations, RainbowlandGame& game);
   void placePickup(RainbowlandGame& game, Vec2f location, BonusType bonus, WeaponType weapon);
   void checkPickups(RainbowlandGame& game);
   void updatePickups(RainbowlandGame& game);

   void selectWeapon(Player& player, WeaponType weapon, const VWeapons& weaponDb);
   void calculateWeaponBonuses(Player& player, const VWeapons& weaponDb);
   void fireWeapons(RainbowlandGame& game);
   void generateBullets(VBullets& bullets, Random& gen, uint32_t count, float spread, Vec2f origin, Vec2f direction, uint32_t damage, bool pierce, void* owner);
   void generateBlast(VBlasts& blasts, Vec2f location, float startRadius, float endRadius, float duration, int32_t damage, void* owner);
   void generateRocket(VRockets& rockets, Vec2f location, Vec2f direction, uint32_t damage, void* owner);
   void moveBullets(VBullets& bullets);
   void moveBlasts(VBlasts& blasts);
   void moveRockets(VRockets& rockets);
   void updateBullets(RainbowlandGame& game);
   void updateBlasts(RainbowlandGame& game);
   void updateRockets(RainbowlandGame& game);
   void increaseKillCount(RainbowlandGame& game, void* owner);

   void updateMonsterSpawners(RainbowlandGame& game);
   void generateMonster(VPMonsters& monsters, Vec2f position, RainbowlandGame& game);
   void runMonsterAI(RainbowlandGame& game);
   void moveMonsters(RainbowlandGame& game);
   void orientMonsters(VPMonsters& monsters);
   void hurtMonster(Monster& monster, uint32_t amount);
   void checkMonsterHurtingPlayer(RainbowlandGame& game);
   void killMonsters(RainbowlandGame& game, VKillLocations& killLocations, bool grantExp);
   void animateMonsters(RainbowlandGame& game);

   void enterPerkMode(RainbowlandGame& game);
   void exitPerkMode(RainbowlandGame& game);
   void generatePerks(VPlayers& players, const VPerks& perkDb);
   bool allPlayersChosePerk(VPlayers& players);
   void applyPerksForPlayers(RainbowlandGame& game);
   void updatePerks(RainbowlandGame& game);

   void activateAbility(Player& player, RainbowlandGame& game);
   uint32_t abilityTimeLeft(RainbowlandGame& game, AbilityType ability);
   float abilityPercentCharged(RainbowlandGame& game, AbilityType ability);
   bool isAbilityActive(RainbowlandGame& game, AbilityType ability);
   
   void activateDefenseMatrix(Player& player, RainbowlandGame& game);
   void activateTimeCapsule(Player& player, RainbowlandGame& game);
   void activateHealingCloud(Player& player, RainbowlandGame& game);
   void activateTurret(Player& player, RainbowlandGame& game);

   void updateDefenseMatrix(RainbowlandGame& game);
   void updateTimeCapsule(RainbowlandGame& game);
   void updateHealingCloud(RainbowlandGame& game);
   void updateTurret(RainbowlandGame& game);

   uint32_t defenseMatrixTimeLeft(RainbowlandGame& game);
   uint32_t healingCloudTimeLeft(RainbowlandGame& game);
   uint32_t timeCapsuleTimeLeft(RainbowlandGame& game);
   uint32_t turretTimeLeft(RainbowlandGame& game);

   void generateSplatter(VKillLocations loc, RainbowlandGame& game);

   Vec2f steer_seek(Vec2f position, float maxSpeed, Vec2f targetLocation);
   Vec2f steer_flee(Vec2f position, float maxSpeed, Vec2f targetLocation);
   Vec2f steer_arrive(Vec2f position, float maxSpeed, Vec2f targetLocation);
   Vec2f steer_pursuit(Vec2f position, Vec2f heading, float maxSpeed, Vec2f targetPosition, Vec2f targetHeading, float targetMaxSpeed);
   Vec2f steer_wander(Monster& m, RainbowlandGame& game);
}
