//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/GameDefinitions.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Rainbowland.h>
#include <Util/CollisionChecks.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
   void initPlayer(Player& player, uint32_t id, RainbowlandGame& game)
   {
      player.id = id;
      player.objectTimer.reset();
      //player.weaponTimer set in selectWeapon
      player.transform.position.set(0 + (float)id, 0);
      player.transform.scale.set(1.0f, 1.0f);
      player.transform.rotation = 0;
      player.collisionData.set(0, 0, 0.8f);
      player.direction.set(1.0f, 0.0f);
      player.targetDirection = player.direction;
      player.directionActive[Up] = player.directionActive[Down] = player.directionActive[Left] = player.directionActive[Right] = false;
      player.currentSpeed = 0;
      player.maxSpeed = 5;
      player.acceleration = 1.0f;
      player.aim = player.direction;
      player.aimDirection = Vec2f::normalize(player.aim);
      player.aimDistance = 0;
      player.maxAimDistance = 6;
      player.minAimDistance = 1.5;
      //player.currentWeapon set in selectWeapon
      player.rateOfFireMultiplier = 1;
      player.reloadMultiplier = 1;
      player.ammoMultiplier = 1;
      player.extraBullets = 0;
      player.dodgeChance = 0;
      player.bonusDamage = 0;
      player.maxHealth = player.health = 100;
      player.regenDelayForOneHealth = 0;
      player.killCount = 0;
      player.perksPerLevel = 3;
      player.selectedPerkIndex = 0;
      for( auto& p : game.m_perkDatabase )
      {
         player.availablePerks.emplace_back(p.type);
      }
      player.chosenPerk = PerkTypeCount;
      player.isShooting = false;
      selectWeapon(player, Pistol, game.m_weaponDatabase);

      float dimm = 0.7f;
      switch( id )
      {
         case 0:
         {
            player.color.set(1, dimm, dimm);
            player.ability = AT_HealingCloud;
            player.skillCooldown = game.m_healingCloud.cooldownSeconds;
         } break;
         case 1:
         {
            player.color.set(1, 1, dimm);
            player.ability = AT_DefenseMatrix;
            player.skillCooldown = game.m_defenseMatrix.cooldownSeconds;
         } break;
         case 2:
         {
            player.color.set(dimm, 1, dimm);
            player.ability = AT_Turret;
            player.skillCooldown = game.m_turret.cooldownSeconds;
         } break;
         case 3:
         {
            player.color.set(dimm, dimm, 1);
            player.ability = AT_TimeCapsule;
            player.skillCooldown = game.m_timeCapsule.cooldownSeconds;
         } break;
      }
   }

   void movePlayers(RainbowlandGame& game)
   {
      for( Player& player : game.m_players )
      {
         if( !player.isAimRelative )
         {
            if( player.directionActive[Up] || player.directionActive[Down] || player.directionActive[Left] || player.directionActive[Right] )
            {
               player.currentSpeed = player.maxSpeed;

               Vec2f targetVector;
               if( player.directionActive[Up] )
               {
                  targetVector.y += 1;
               }
               if( player.directionActive[Down] )
               {
                  targetVector.y -= 1;
               }
               if( player.directionActive[Left] )
               {
                  targetVector.x -= 1;
               }
               if( player.directionActive[Right] )
               {
                  targetVector.x += 1;
               }

               player.targetDirection = Vec2f::normalize(targetVector);
            }
            else
            {
               player.currentSpeed = 0;
               player.targetDirection = player.direction;
            }
         }

         float currentRad = std::atan2f(player.direction.y, player.direction.x);
         float targetRad = std::atan2f(player.targetDirection.y, player.targetDirection.x);
         float currentDeg = Rad2Deg(currentRad);
         float targetDeg = Rad2Deg(targetRad);

         auto wra = targetDeg - currentDeg;
         if( wra > 180.0f ) wra -= 360.0f;
         if( wra < -180.0f ) wra += 360.0f;

         currentDeg += wra * 4 * player.objectTimer.getDeltaSeconds();
         currentRad = Deg2Rad(currentDeg);

         player.direction.x = std::cosf(currentRad);
         player.direction.y = std::sinf(currentRad);

         auto velocity = player.direction*player.currentSpeed;
         player.transform.position += (player.objectTimer.getDeltaSeconds()*velocity);
         Circle playerCollider = player.collisionData;
         playerCollider.center = player.transform.position;
         if( !isCircleInsideRect(playerCollider, game.m_playingField) )
         {
            clamp(game.m_playingField.left() + player.collisionData.radius,
                  game.m_playingField.right() - player.collisionData.radius,
                  player.transform.position.x);
            clamp(game.m_playingField.bottom() + player.collisionData.radius,
                  game.m_playingField.top() - player.collisionData.radius,
                  player.transform.position.y);
         }
      }
   }

   void orientPlayers(VPlayers& players)
   {
      for( auto& player : players )
      {
         auto aimDir = player.aim;
         if( !player.isAimRelative )
         {
            aimDir -= player.transform.position;
         }
         aimDir = Vec2f::normalize(aimDir);
         player.transform.rotation = std::atan2(aimDir.y, aimDir.x);
      }
   }

   void updatePlayerAim(VPlayers& players)
   {
      for( auto& player : players )
      {
         if( player.isAimRelative )
         {
            player.aim = player.aimDirection*(player.minAimDistance + player.aimDistance*player.maxAimDistance);
         }
      }
   }

   void checkPlayerDeath(RainbowlandGame& game)
   {
      for( uint32_t p = 0; p < game.m_players.size(); )
      {
         if( game.m_players[p].health <= 0 )
         {
            //    game.m_guiSystem.removeElement("player" + std::to_string(game.m_players[p].id));
            generateBlast(game.m_blasts, game.m_players[p].transform.position, 0, 3, 3, 50, &game.m_players[p]);
            game.m_deadPlayers.emplace_back(game.m_players[p]);
            game.m_players[p] = game.m_players.back();
            game.m_players.pop_back();
         }
         else
         {
            ++p;
         }
      }
   }

   void checkLevelup(RainbowlandGame& game)
   {
      if( game.m_experience >= game.m_experienceForNextLevel && !game.m_players.empty() )
      {
         game.m_experienceForNextLevel += game.m_experienceIncrement;
         ++game.m_level;
         enterPerkMode(game);
         for( auto& player : game.m_players )
         {
            generateBlast(game.m_blasts, player.transform.position, 0, 2, 1, 30, &player);
         }
      }
   }

   void grantExperience(uint32_t exp, RainbowlandGame& game)
   {
      game.m_experience += exp;
   }

   void fixupCamera(RainbowlandGame& game)
   {
      if( game.m_players.size() > 0 )
      {
         Vec2f averagePos;
         for( auto& player : game.m_players )
         {
            averagePos += player.transform.position;
         }
         averagePos /= (float)game.m_players.size();
         auto pos = game.m_camera.getPosition();
         Vec2f pos2{pos.x, pos.y};
         clamp(-game.m_cameraBounds.x, game.m_cameraBounds.x, averagePos.x);
         clamp(-game.m_cameraBounds.y, game.m_cameraBounds.y, averagePos.y);
         auto diff = averagePos - pos2;
         if( Vec2f::length(diff) > 0 )
         {
            pos.set(averagePos.x, averagePos.y, pos.z);
            game.m_camera.move({diff.x, diff.y, 0});
            game.m_players[0].aim += diff;
         }
      }
   }

   void updateDifficulty(RainbowlandGame& game)
   {
      game.m_difficultyTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      if( game.m_difficultyTimer.hasElapsed() )
      {
         game.m_difficultyTimer.period();
         game.m_difficulty += (0.1f / game.m_playerCount);
      }
      if( game.m_killCounter >= 100 )
      {
         game.m_difficulty += (0.5f / game.m_playerCount);
         game.m_killCounter -= 100;
      }
   }

   void enableBonus(BonusType bonus, Vec2f pickupPosition, Player& player, RainbowlandGame& game)
   {
      if( game.m_bonusDatabase[bonus].acquireLogic )
      {
         game.m_bonusDatabase[bonus].acquireLogic(pickupPosition, player, game);
      }
   }

   void disableBonus(BonusType bonus, RainbowlandGame& game)
   {
      if( game.m_bonusDatabase[bonus].timeoutLogic )
      {
         game.m_bonusDatabase[bonus].timeoutLogic(game);
      }
   }

   void updateBonuses(RainbowlandGame& game)
   {
      for( uint32_t b = 0; b < game.m_activeBonuses.size(); )
      {
         auto& bonus = game.m_activeBonuses[b];
         if( bonus.timer.hasElapsed() )
         {
            disableBonus(bonus.type, game);
         }
         else
         {
            ++b;
         }
      }
   }

   void generatePickups(VKillLocations& killLocations, RainbowlandGame& game)
   {
      if( game.m_pickups.size() >= 7 )
      {
         return;
      }

      for( auto& loc : killLocations )
      {
         //check if too far away from center
         auto modifiedField = game.m_playingField;
         modifiedField.halfHeight *= 0.8f;
         modifiedField.halfWidth *= 0.8f;
         if( !isPointInsideRect(loc.transform.position, modifiedField) )
         {
            continue;
         }

         //check if there is any other bonus at this location
         bool shouldItSpawn = true;
         for( auto& pick : game.m_pickups )
         {
            Circle old = pick.collisionData;
            old.center = pick.transform.position;
            Circle nju = pick.collisionData;
            nju.center = loc.transform.position;
            if( isCircleTouchingCircle(old, nju) )
            {
               shouldItSpawn = false;
               break;
            }
         }
         if( shouldItSpawn )
         {
            for( auto& player : game.m_players )
            {
               Circle plc{player.transform.position, player.collisionData.radius * 2};
               Circle pic{loc.transform.position, 1.1f};
               if( isCircleTouchingCircle(plc, pic) )
               {
                  shouldItSpawn = false;
                  break;
               }
            }
         }
         if( shouldItSpawn == false )
         {
            continue;
         }

         int32_t weaponChance = 5;
         int32_t bonusChance = 5;

         if( std::any_of(game.m_players.begin(), game.m_players.end(),
            [](Player& p) { return p.currentWeapon.type == Pistol; }) )
         {
            weaponChance = 90;
         }
         bool spawned = false;
         if( game.m_randomGenerator.randInt(1, 100 * 100) < weaponChance * 100 )
         {
            uint32_t count = std::count_if(game.m_pickups.begin(), game.m_pickups.end(),
                                       [](Pickup& p) { return p.weapon != WeaponTypeCount; });
            if( count == game.m_playerCount )
            {
               return;
            }
            WeaponType weapon = (WeaponType)game.m_randomGenerator.randInt(1, game.m_weaponDatabase.size() - 1);
            placePickup(game, loc.transform.position, BonusTypeCount, weapon);
         }
         if( !spawned && (game.m_randomGenerator.randInt(1, 100 * 100) < bonusChance * 100) )
         {
            BonusType bonus = (BonusType)game.m_randomGenerator.randInt(0, game.m_bonusDatabase.size() - 1);
            placePickup(game, loc.transform.position, bonus, WeaponTypeCount);
         }
      }
   }

   void placePickup(RainbowlandGame& game, Vec2f location, BonusType bonus, WeaponType weapon)
   {
      bool exists = std::find_if(game.m_pickups.begin(), game.m_pickups.end(),
         [=](Pickup& p) { return p.bonus == SlowTime; }) != game.m_pickups.end();

      game.m_pickups.emplace_back();
      game.m_pickups.back().objectTimer.setDurationMicros(secondsToMicros(15U));
      game.m_pickups.back().transform.position = location;
      game.m_pickups.back().transform.scale.set(1.1f, 1.1f);
      game.m_pickups.back().collisionData.set(0, 0, 1);
      game.m_pickups.back().bonus = bonus;
      game.m_pickups.back().weapon = weapon;

      if( bonus != BonusTypeCount )
      {
         BonusIcon icons[BonusTypeCount] =
         {
            AtomicBombIcon,
            SlowTimeIcon,
            HealIcon,
            MassHealIcon,
            IncreasedMovementSpeedIcon,
            IncreasedRateOfFireIcon
         };

         if (bonus == SlowTime && exists)
         {
            game.m_pickups.pop_back();
            return;
         }

         game.m_pickups.back().iconIndex = icons[bonus] + game.m_imageStartIndex_bonuses;
      }
      else
      {
         WeaponIcon icons[WeaponTypeCount] =
         {
            PistolIcon,
            ShotgunIcon,
            UziIcon,
            SniperIcon,
            RPGIcon,
         };

         game.m_pickups.back().iconIndex = game.m_imageStartIndex_bonuses + icons[weapon];
      }

   }

   void checkPickups(RainbowlandGame& game)
   {
      for( auto& player : game.m_players )
      {
         for( uint32_t b = 0; b < game.m_pickups.size(); )
         {
            Circle pCollider = player.collisionData;
            pCollider.center = player.transform.position;
            pCollider.radius *= player.transform.scale.x;
            Circle bCollider = game.m_pickups[b].collisionData;
            bCollider.center = game.m_pickups[b].transform.position;
            bCollider.radius *= game.m_pickups[b].transform.scale.x;
            if( isCircleTouchingCircle(pCollider, bCollider) )
            {
               if( game.m_pickups[b].bonus != BonusTypeCount )
               {
                  enableBonus(game.m_pickups[b].bonus, bCollider.center, player, game);
               }
               if( game.m_pickups[b].weapon != WeaponTypeCount )
               {
                  selectWeapon(player, game.m_pickups[b].weapon, game.m_weaponDatabase);
               }
               game.m_pickups[b] = game.m_pickups.back();
               game.m_pickups.pop_back();
            }
            else
            {
               ++b;
            }
         }
      }
   }

   void updatePickups(RainbowlandGame& game)
   {
      for( uint32_t b = 0; b < game.m_pickups.size(); )
      {
         if( game.m_pickups[b].objectTimer.hasElapsed() )
         {
            game.m_pickups[b] = game.m_pickups.back();
            game.m_pickups.pop_back();
         }
         else
         {
            auto diff = 1 - game.m_pickups[b].objectTimer.getPercentDone();
            game.m_pickups[b].color.a = diff / 0.15f;
            clamp(0.0f, 1.0f, game.m_pickups[b].color.a);
            ++b;
         }
      }
   }

   void selectWeapon(Player& player, WeaponType weapon, const VWeapons& weaponDb)
   {
      player.currentWeapon = weaponDb[weapon];
      calculateWeaponBonuses(player, weaponDb);
      player.currentWeapon.ammo = player.currentWeapon.maxAmmo;
      player.weaponTimer.setDurationMicros(player.currentWeapon.fireDelay);
      player.weaponTimer.updateBy(player.weaponTimer.getDurationMicros());
   }

   void calculateWeaponBonuses(Player& player, const VWeapons& weaponDb)
   {
      const auto& dbw = weaponDb[player.currentWeapon.type];
      player.currentWeapon.damage = player.bonusDamage + dbw.damage;
      player.currentWeapon.fireDelay = static_cast<uint64_t>(player.rateOfFireMultiplier*static_cast<float>(dbw.fireDelay));
      player.currentWeapon.maxAmmo = static_cast<uint32_t>(player.ammoMultiplier*static_cast<float>(dbw.maxAmmo));
      player.currentWeapon.maxAmmo += player.extraBullets;
      player.currentWeapon.reloadDelay = static_cast<uint64_t>(player.reloadMultiplier*static_cast<float>(dbw.reloadDelay));
      if( player.currentWeapon.ammo > 0 )
      {
         player.weaponTimer.setDurationMicros(player.currentWeapon.fireDelay);
      }
      else
      {
         player.weaponTimer.setDurationMicros(player.currentWeapon.reloadDelay);
      }
   }

   void fireWeapons(RainbowlandGame& game)
   {
      for( auto& player : game.m_players )
      {
         auto& w = player.currentWeapon;
         if( w.ammo > 0 )
         {
            player.weaponTimer.updateBy(player.objectTimer.getDeltaMicros());
            if( player.isShooting && player.weaponTimer.hasElapsed() )
            {
               player.weaponTimer.reset();
               --w.ammo;
               if( w.ammo == 0 )
               {
                  player.weaponTimer.setDurationMicros(w.reloadDelay);
               }
               auto aim = player.aim;
               if( !player.isAimRelative )
               {
                  aim -= player.transform.position;
               }
               aim = Vec2f::normalize(aim);
               auto p = aim*player.collisionData.radius;

               switch( w.type )
               {
                  case RPG:
                  {
                     generateRocket(game.m_rockets, player.transform.position, aim, w.damage, &player);

                  } break;

                  case Pistol:
                  case Shotgun:
                  case Uzi:
                  case Sniper:
                  {
                     generateBullets(game.m_bullets, game.m_randomGenerator, w.bulletsPerShot,
                                     w.spread, player.transform.position + p, aim, w.damage, w.bulletPierce, &player);
                  } break;
               }

               game.m_audioSystem.playSfx(game.m_shootSounds[w.type]);
            }
         }
         else
         {
            if( player.weaponTimer.getPercentDone() == 0 )
            {
               game.m_audioSystem.playSfx(game.m_reloadSfx);
            }
            player.weaponTimer.updateBy(player.objectTimer.getDeltaMicros());
            if( player.weaponTimer.hasElapsed() )
            {
               w.ammo = w.maxAmmo;
               player.weaponTimer.setDurationMicros(w.fireDelay);
               player.weaponTimer.reset();
            }
         }
      }
   }

   void generateBullets(VBullets& bullets, Random& gen, uint32_t count, float spread, Vec2f origin, Vec2f direction, uint32_t damage, bool pierce, void* owner)
   {
      Vec2f targetDistance = direction * 30;
      for( uint32_t i = 0; i < count; ++i )
      {
         Vec2f t{gen.randFloat()*spread * 2 - spread, gen.randFloat()*spread * 2 - spread};
         Bullet b;
         b.oldPosition = b.position = b.trail = origin;
         b.velocity = Vec2f::normalize(targetDistance + t) * 60;
         b.travelled = 0;
         b.damage = damage;
         b.owner = owner;
         b.pierce = pierce;
         b.dead = false;
         bullets.emplace_back(b);
      }
   }

   void generateBlast(VBlasts& blasts, Vec2f location, float startRadius, float endRadius, float duration, int32_t damage, void* owner)
   {
      Blast blast;
      blast.area.center = location;
      blast.area.radius = startRadius;
      blast.maxRadius = endRadius;
      blast.rotation = 0;
      blast.damage = damage;
      blast.objectTimer.setDurationMicros(secondsToMicros(duration));
      blast.owner = owner;
      blasts.emplace_back(blast);
   }

   void generateRocket(VRockets& rockets, Vec2f location, Vec2f direction, uint32_t damage, void* owner)
   {
      Rocket rocky;
      rocky.objectTimer.setDurationMicros(24 * 1000000U);
      rocky.body.center = location;
      rocky.body.radius = 0.2f;
      rocky.direction = direction;
      rocky.speed = 20;
      rocky.damage = damage;
      rocky.owner = owner;
      rockets.emplace_back(rocky);
   }

   void moveBullets(VBullets& bullets)
   {
      for( uint32_t i = 0; i < bullets.size(); ++i )
      {
         auto displacement = bullets[i].objectTimer.getDeltaSeconds()*bullets[i].velocity;
         auto travel = Vec2f::length(displacement);
         bullets[i].travelled += travel;
         if( !bullets[i].dead )
         {
            bullets[i].oldPosition = bullets[i].position;
            bullets[i].position += displacement;
         }
         if( bullets[i].travelled > 10 )
         {
            bullets[i].trail += (displacement*(1.0f + (bullets[i].dead ? 1.0f : 0.0f)));
         }

         if( bullets[i].travelled > 50 )
         {
            bullets[i].dead = true;
         }
         if( bullets[i].dead )
         {
            auto old = Vec2f::normalize(bullets[i].oldPosition - bullets[i].position);
            auto trail = Vec2f::normalize(bullets[i].trail - bullets[i].position);
            auto dot = Vec2f::dotProduct(old, trail);
            if( dot < 0 )
            {
               bullets[i] = bullets.back();
               bullets.pop_back();
               --i;
            }
         }
      }
   }

   void moveBlasts(VBlasts& blasts)
   {
      for( auto& blast : blasts )
      {
         auto percent = blast.objectTimer.getPercentDone();
         blast.area.radius = blast.maxRadius*percent;
         blast.rotation += Deg2Rad(5);
      }
   }

   void moveRockets(VRockets& rockets)
   {
      for( auto& rocket : rockets )
      {
         rocket.body.center += rocket.direction*rocket.speed*rocket.objectTimer.getTimer().getDeltaSeconds();
      }
   }

   void findBulletHits(const Bullet& bullet, RainbowlandGame& game, std::vector<Monster*>& outMonsters)
   {
      auto oldPosCell = calculateCellCoords(game.m_monsterGrid, bullet.oldPosition);
      auto posCell = calculateCellCoords(game.m_monsterGrid, bullet.position);
      auto monsters = collectMonstersInArea(game.m_monsterGrid, oldPosCell, {1, 1});
      if( oldPosCell != posCell )
      {
         auto r2 = collectMonstersInArea(game.m_monsterGrid, posCell, {1, 1});
         monsters.insert(monsters.end(), r2.begin(), r2.end());
         std::sort(monsters.begin(), monsters.end());
         auto newEnd = std::unique(monsters.begin(), monsters.end());
         monsters.erase(newEnd, monsters.end());
      }
      for( auto* monster : monsters )
      {
         if( monster->health > 0 )
         {
            Circle bCollider = monster->collisionData_hitbox;
            bCollider.center = monster->transform.position;
            bCollider.radius *= monster->transform.scale.x;
            if( isLineTouchingCircle(bullet.oldPosition, bullet.position, bCollider) )
            {
               outMonsters.push_back(monster);
            }
         }
      }
   }

   void findBlastHits(const Blast& blast, RainbowlandGame& game, std::vector<Monster*>& outMonsters)
   {
      auto posCell = calculateCellCoords(game.m_monsterGrid, blast.area.center);
      auto minSize = std::min(game.m_monsterGrid.cellHalfsize.x, game.m_monsterGrid.cellHalfsize.y);
      auto extraCells = (int32_t)(blast.area.radius / minSize);
      auto monsters = collectMonstersInArea(game.m_monsterGrid, posCell, {1 + extraCells, 1 + extraCells});

      for( auto* monster : monsters )
      {
         if( monster->health > 0 )
         {
            Circle bCollider = monster->collisionData_hitbox;
            bCollider.center = monster->transform.position;
            bCollider.radius *= monster->transform.scale.x;
            if( isCircleTouchingCircle(bCollider, blast.area) )
            {
               outMonsters.push_back(monster);
            }
         }
      }
   }

   void findRocketHits(const Rocket& rock, RainbowlandGame& game, std::vector<Monster*>& outMonsters)
   {
      auto posCell = calculateCellCoords(game.m_monsterGrid, rock.body.center);
      auto minSize = std::min(game.m_monsterGrid.cellHalfsize.x, game.m_monsterGrid.cellHalfsize.y);
      auto extraCells = (int32_t)(rock.body.radius / minSize);
      auto monsters = collectMonstersInArea(game.m_monsterGrid, posCell, {1 + extraCells, 1 + extraCells});

      for( auto* monster : monsters )
      {
         if( monster->health > 0 )
         {
            Circle bCollider = monster->collisionData_hitbox;
            bCollider.center = monster->transform.position;
            bCollider.radius *= monster->transform.scale.x;
            if( isCircleTouchingCircle(bCollider, rock.body) )
            {
               outMonsters.push_back(monster);
            }
         }
      }
   }

   void updateBullets(RainbowlandGame& game)
   {
      for( uint32_t b = 0; b < game.m_bullets.size(); ++b )
      {
         auto& bullet = game.m_bullets[b];
         if( !bullet.dead )
         {
            std::vector<Monster*> hitMonsters;
            findBulletHits(bullet, game, hitMonsters);
            if( !hitMonsters.empty() )
            {
               std::sort(hitMonsters.begin(), hitMonsters.end(), [&](Monster* l, Monster* r)
               {
                  auto ll = Vec2f::length(l->transform.position - bullet.trail);
                  auto rl = Vec2f::length(r->transform.position - bullet.trail);
                  return ll < rl;
               });
               for( auto* monster : hitMonsters )
               {
                  hurtMonster(*monster, bullet.damage);
                  if( monster->health <= 0 )
                  {
                     //notify owner of the kill
                     increaseKillCount(game, bullet.owner);
                  }
                  if( !bullet.pierce )
                  {
                     break;
                  }
               }
            }
            if( !hitMonsters.empty() && !bullet.pierce )
            {
               game.m_bullets[b].dead = true;
            }
         }
      }
   }

   void updateBlasts(RainbowlandGame& game)
   {
      for( uint32_t blastIndex = 0; blastIndex < game.m_blasts.size(); )
      {
         auto& blast = game.m_blasts[blastIndex];
         bool done = blast.maxRadius <= blast.area.radius;
         if( !done )
         {
            //get monsters inside radius
            std::vector<Monster*> hitMonsters;
            findBlastHits(blast, game, hitMonsters);
            if( !hitMonsters.empty() )
            {
               for( auto* monster : hitMonsters )
               {
                  hurtMonster(*monster, blast.damage);
                  if( monster->health <= 0 )
                  {
                     increaseKillCount(game, blast.owner);
                  }
               }
            }

            ++blastIndex;
         }
         else
         {
            //remove
            game.m_blasts[blastIndex] = game.m_blasts.back();
            game.m_blasts.pop_back();
         }
      }
   }

   void updateRockets(RainbowlandGame& game)
   {
      for( uint32_t rocketIndex = 0; rocketIndex < game.m_rockets.size(); )
      {
         auto& rock = game.m_rockets[rocketIndex];
         bool done = rock.objectTimer.hasElapsed();
         if( !done )
         {
            //get monsters inside radius
            std::vector<Monster*> hitMonsters;
            findRocketHits(rock, game, hitMonsters);
            if( !hitMonsters.empty() )
            {
               generateBlast(game.m_blasts, rock.body.center, 0, 2, 0.5f, rock.damage, rock.owner);
               done = true;
            }
            else
            {
               ++rocketIndex;
            }
         }

         if( done )
         {
            //remove
            game.m_rockets[rocketIndex] = game.m_rockets.back();
            game.m_rockets.pop_back();
         }
      }
   }

   void increaseKillCount(RainbowlandGame& game, void* owner)
   {
      ++game.m_totalKillCount;
      ++game.m_killCounter;
      if( owner == nullptr )
         return;

      if( owner == &game.m_turret )
      {
         ++game.m_turret.killCount;
      }
      else
      {
         for( auto& player : game.m_players )
         {
            if( owner == &player )
            {
               ++player.killCount;
            }
         }
      }
   }

   void updateMonsterSpawners(RainbowlandGame& game)
   {
      if( game.m_players.size() == 0 ) return;

      for( auto& spawner : game.m_monsterSpawners )
      {
         if( spawner.spawnTimer.hasElapsed() )
         {
            spawner.spawnTimer.period();

            Vec2f displacement{(game.m_randomGenerator.randFloat() * 2 - 1)*spawner.spawnRadius,
               (game.m_randomGenerator.randFloat() * 2 - 1)*spawner.spawnRadius};
            generateMonster(game.m_monsters,
                            spawner.transform.position + displacement,
                            game);

         }
      }
   }

   void scaleMonsterToDifficulty(Monster& monsta, RainbowlandGame& game)
   {
      monsta.health = static_cast<uint32_t>(static_cast<float>(monsta.health)*game.m_difficulty);
      clamp(1, 400, monsta.health);
      monsta.maxHealth = monsta.health;
      monsta.damage = static_cast<uint32_t>(static_cast<float>(monsta.damage)*game.m_difficulty);
      clamp(1U, 15U, monsta.damage);
      monsta.maxSpeed *= game.m_difficulty;
      clamp(0.0f, 5.0f, monsta.maxSpeed);
   }

   void setFlowerSpecificData(Monster& monster)
   {
      monster.collisionData_attack.set(0, 0, 0.6f);
      monster.collisionData_hitbox.set(0, 0, 0.7f);
      //monster.brain.state = BS_Attack;
   }

   void setLadybugSpecificData(Monster& monster)
   {
      monster.collisionData_attack.set(0, 0, 0.7f);
      monster.collisionData_hitbox.set(0, 0, 0.8f);
      //monster.brain.state = BS_Flank;
   }

   void setButterflySpecificData(Monster& monster)
   {
      monster.collisionData_attack.set(0, 0, 0.5f);
      monster.collisionData_hitbox.set(0, 0, 0.7f);
      //monster.brain.state = BS_Wander;
   }

   void generateMonster(VPMonsters& monsters, Vec2f position, RainbowlandGame& game)
   {
      auto* monster_ptr = game.m_monsterPool.newMonster();
      if( monster_ptr == nullptr )
      {
         return;
      }

      monsters.emplace_back(monster_ptr);
      auto& monster = *monsters.back();

      monster.objectTimer.reset();
      monster.maxSpeed = 1 + game.m_randomGenerator.randFloat() * 1.5f;
      monster.turnSpeed = 1 + monster.maxSpeed;
      monster.direction.set(1, 0);
      monster.transform.position = position;
      auto scale = 1.3f - game.m_randomGenerator.randFloat()*0.6f;
      monster.transform.scale.set(scale, scale);
      monster.collisionData_separation.set(0, 0, 0.5f);
      monster.brain.timer.setPeriodMicros(secondsToMicros(game.m_randomGenerator.randFloat()+0.2f));
      monster.brain.timer.updateBy(monster.brain.timer.getPeriodMicros());
      monster.brain.targetPlayer = UINT32_MAX;
      monster.brain.targetLocation = position;
      monster.gridCell.set(10000000, 10000000);
      auto rnd = game.m_randomGenerator.randInt(0, 100 * 100);
      if( rnd < 20 * 100 )
         monster.brain.state = BS_Wander;
      else if( 20 * 100 <= rnd && rnd < 60 * 100 )
         monster.brain.state = BS_Flank;
      else
         monster.brain.state = BS_Attack;
      monster.brain.attackRadius = 7;
      monster.brain.chanceToWander = 0.05f;
      uint32_t hp = static_cast<uint32_t>(30 * (game.m_randomGenerator.randFloat()*0.4f + 0.8f));
      monster.maxHealth = monster.health = static_cast<uint32_t>(hp);
      monster.attackTimer.setDurationMicros(secondsToMicros(0.5f));
      monster.attackTimer.updateBy(monster.attackTimer.getDurationMicros());
      monster.damage = 1;
      monster.expGain = hp;
      monster.animationTimer.setPeriodMicros(milisToMicros(100U));
      monster.currentAnimationFrame = 0;
      monster.color.set(1, 1, 1);
      if( game.m_randomGenerator.randInt(0, 100 * 100) < 15 * 100 )
      {
         monster.expGain *= 2;
         monster.color.set(0.7f, 1, 0.7f);
      }
      scaleMonsterToDifficulty(monster, game);

      monster.type = (MonsterType)game.m_randomGenerator.randInt(0, MonsterTypeCount - 1);
      switch( monster.type )
      {
         case Flower:
         {
            setFlowerSpecificData(monster);
         } break;
         case Ladybug:
         {
            setLadybugSpecificData(monster);
         } break;
         case Butterfly:
         {
            setButterflySpecificData(monster);
         } break;
      }

      updateMonsterInGrid(game.m_monsterGrid, monster);
   }

   uint32_t findPlayerInArea(Circle area, VPlayers& players)
   {
      auto index = 0;
      for( auto& player : players )
      {
         if( isPointInsideCircle(player.transform.position, area) )
         {
            return index;
         }
         ++index;
      }
      return players.size();
   }

   void runMonsterAI(RainbowlandGame& game)
   {
      for( auto* monster_ptr : game.m_monsters )
      {
         auto& monster = *monster_ptr;
         monster.brain.timer.updateBy(monster.objectTimer.getDeltaMicros());
         if( monster.brain.timer.hasElapsed() )
         {
            monster.brain.timer.period();
            //monster brain does work here
            //decide which player to target
            if( game.m_players.size() == 0 )
            {
               monster.brain.state = BS_Wander;
            }

            auto playerId = monster.brain.targetPlayer;
            auto playerIndex = filterFind(game.m_players, [=](const Player& p) { return p.id == playerId; });
            if( playerIndex == game.m_players.size() && playerIndex != 0 )
            {
               playerIndex = game.m_randomGenerator.randInt(0, game.m_players.size() - 1);
               monster.brain.targetPlayer = game.m_players[playerIndex].id;
            }

            switch( monster.brain.state )
            {
               case BS_Wander:
               {
                  auto direction = steer_wander(monster, game);
                  auto circle = monster.collisionData_hitbox;
                  circle.center = monster.transform.position;
                  if( !isCircleInsideRect(circle, game.m_playingField) )
                  {
                     direction = Vec2f::normalize(-monster.transform.position);
                  }
                  monster.brain.targetLocation = monster.transform.position + direction * 4;
                  auto pi = findPlayerInArea(Circle{monster.transform.position, monster.brain.attackRadius}, game.m_players);
                  if( pi != game.m_players.size())
                  {
                     monster.brain.targetPlayer = game.m_players[pi].id;
                     monster.brain.state = BS_Attack;
                  }
                  /* if (game.m_randomGenerator.randFloat() < 0.1f && !game.m_players.empty())
                  {
                     pi = game.m_randomGenerator.randInt(0, game.m_players.size() - 1);
                     monster.brain.targetPlayer = game.m_players[pi].id;
                     monster.brain.state = BS_Attack;
                  } */
               } break;

               case BS_Attack:
               {
                  monster.brain.targetLocation = game.m_players[playerIndex].transform.position;
                  auto pi = findPlayerInArea(Circle{monster.transform.position, monster.brain.attackRadius}, game.m_players);
                  if( pi == game.m_players.size() && game.m_randomGenerator.randFloat() < monster.brain.chanceToWander )
                  {
                     monster.brain.state = BS_Wander;
                  }
               } break;

               case BS_Flank:
               {
                  auto& player = game.m_players[playerIndex];
                  auto target = Vec2f{game.m_randomGenerator.randFloat() * 2 - 1,
                     game.m_randomGenerator.randFloat() * 2 - 1};
                  target = Vec2f::normalize(target) * 4;
                  target += player.transform.position;

                  monster.brain.targetLocation = target;
                  auto pi = findPlayerInArea(Circle{monster.transform.position, monster.brain.attackRadius}, game.m_players);
                  if( pi != game.m_players.size() )
                  {
                     monster.brain.targetPlayer = game.m_players[pi].id;
                     monster.brain.state = BS_Attack;
                  }
               } break;
            }
         }
      }
   }

   void separateMonsters(Monster& monsta, Grid& grid)
   {
      auto cell = calculateCellCoords(grid, monsta.transform.position);
      auto monsters = collectMonstersInArea(grid, cell, {1, 1});
      for( auto* monster : monsters )
      {
         if( &monsta == monster ) continue;

         auto distanceVec = monsta.transform.position - monster->transform.position;
         auto distanceLen = Vec2f::length(distanceVec);

         auto overlap = monsta.collisionData_separation.radius*monsta.transform.scale.x +
            monster->collisionData_separation.radius*monster->transform.scale.x - distanceLen;
         if( overlap > 0 )
         {
            monsta.transform.position += (overlap*distanceVec / distanceLen);
         }
      }
      updateMonsterInGrid(grid, monsta);
   }

   void moveMonsters(RainbowlandGame& game)
   {
      for( auto* monster_ptr : game.m_monsters )
      {
         auto& monster = *monster_ptr;
         auto n = Vec2f::length(steer_arrive(monster.transform.position, monster.maxSpeed, monster.brain.targetLocation));
         monster.transform.position += monster.direction*n*monster.objectTimer.getDeltaSeconds();
      
         updateMonsterInGrid(game.m_monsterGrid, monster);
         separateMonsters(monster, game.m_monsterGrid);
      }
   }

   void animateMonsters(RainbowlandGame& game)
   {
      for( auto* monster_ptr : game.m_monsters )
      {
         monster_ptr->animationTimer.updateBy(monster_ptr->objectTimer.getDeltaMicros());
         if( monster_ptr->animationTimer.hasElapsed() )
         {
            monster_ptr->animationTimer.period();
            ++monster_ptr->currentAnimationFrame;
            switch( monster_ptr->type )
            {
               case Ladybug:
               {
                  monster_ptr->currentAnimationFrame %= game.ladybugAnimationLoop.size();
               } break;
               case Flower:
               {
                  monster_ptr->currentAnimationFrame %= game.flowerAnimationLoop.size();
               } break;
               case Butterfly:
               {
                  monster_ptr->currentAnimationFrame %= game.butterflyAnimationLoop.size();
               } break;
            }
         }
      }
   }

   void orientMonsters(VPMonsters& monsters)
   {
      for( auto* monster_ptr : monsters )
      {
         auto& monster = *monster_ptr;
         float direction = std::atan2f(monster.direction.y, monster.direction.x);
         auto targetDirection = Vec2f::normalize(monster.brain.targetLocation - monster.transform.position);
         float target = std::atan2f(targetDirection.y, targetDirection.x);
         direction = Rad2Deg(direction);
         target = Rad2Deg(target);

         auto wra = target - direction;
         if( wra > 180.0f ) wra -= 360.0f;
         if( wra < -180.0f ) wra += 360.0f;

         auto newDir = direction + wra*monster.turnSpeed*monster.objectTimer.getDeltaSeconds();
         newDir = Deg2Rad(newDir);
         monster.direction.x = std::cosf(newDir);
         monster.direction.y = std::sinf(newDir);

         monster.transform.rotation = newDir;
      }
   }

   void hurtMonster(Monster& monster, uint32_t amount)
   {
      monster.health -= amount;
   }

   void checkMonsterHurtingPlayer(RainbowlandGame& game)
   {
      for( auto& player : game.m_players )
      {
         auto pCollider = player.collisionData;
         pCollider.center = player.transform.position;
         pCollider.radius *= player.transform.scale.x;

         auto cell = calculateCellCoords(game.m_monsterGrid, pCollider.center);
         auto monsters = collectMonstersInArea(game.m_monsterGrid, cell, {1, 1});

         for( auto* monster : monsters )
         {
            auto mCollider = monster->collisionData_attack;
            mCollider.center = monster->transform.position;
            mCollider.radius *= monster->transform.scale.x;
            monster->attackTimer.updateBy(monster->objectTimer.getDeltaMicros());
            if( isCircleTouchingCircle(mCollider, pCollider) )
            {
               if( monster->attackTimer.hasElapsed() )
               {
                  if( game.m_randomGenerator.randInt(0, 100 * 100) >= (int32_t)player.dodgeChance * 100 )
                  {
                     player.health -= monster->damage;
                  }
                  monster->attackTimer.reset();
               }
            }
         }
      }
   }

   void killMonsters(RainbowlandGame& game, VKillLocations& killLocations, bool grantExp)
   {
      for( uint32_t m = 0; m < game.m_monsters.size(); )
      {
         auto& monster = *game.m_monsters[m];
         if( monster.health <= 0 )
         {
            if( grantExp )
            {
               grantExperience(monster.expGain, game);
            }
            killLocations.emplace_back(KillLocation{monster.transform, monster.type});
            auto cell = calculateCellCoords(game.m_monsterGrid, monster.transform.position);
            removeMonsterFromGrid(game.m_monsterGrid, monster);
            game.m_monsterPool.releaseMonster(&monster);
            game.m_monsters[m] = game.m_monsters.back();
            game.m_monsters.pop_back();
         }
         else
         {
            ++m;
         }
      }
   }

   void enterPerkMode(RainbowlandGame& game)
   {
      game.m_enteringPerkMode = true;
      for( auto& player : game.m_players )
      {
         /*player.directionActive[Up] =
            player.directionActive[Down] =
            player.directionActive[Left] =
            player.directionActive[Right] = false;
            player.currentSpeed = 0;*/
         player.targetDirection = player.direction;
      }
   }

   void generatePerks(VPlayers& players, const VPerks& perkDb)
   {
      Random gen{Clock::getRealTimeMicros()};

      for( auto& player : players )
      {
         if( !player.selectablePerks.empty() ) continue;

         player.selectedPerkIndex = 0;
         player.chosenPerk = PerkTypeCount;
         auto perks = player.availablePerks;
         for( uint32_t i = 0; i < player.perksPerLevel; ++i )
         {
            if( perks.size() != 0 )
            {
               uint32_t perkIndex = gen.randInt(0, perks.size() - 1);
               if( perkDb[perks[perkIndex]].dependencyCheck == nullptr ||
                  perkDb[perks[perkIndex]].dependencyCheck(player) )
               {
                  player.selectablePerks.emplace_back(perks[perkIndex]);
               }
               else
               {
                  --i;
               }
               perks[perkIndex] = perks.back();
               perks.pop_back();
            }
            else
            {
               player.selectablePerks.emplace_back(InstantWinner);
            }
         }
      }
   }

   bool allPlayersChosePerk(VPlayers& players)
   {
      return std::all_of(players.begin(), players.end(),
                         [](const Player& p)
      {
         return p.chosenPerk != PerkTypeCount;
      });
   }

   void applyPerksForPlayers(RainbowlandGame& game)
   {
      for( auto& player : game.m_players )
      {
         auto& perk = game.m_perkDatabase[player.chosenPerk];
         if( perk.acquireLogic )
         {
            perk.acquireLogic(player, game);
         }

         player.selectablePerks.clear();
         if( !perk.repeatable )
         {
            player.acquiredPerks.emplace_back();
            player.acquiredPerks.back().type = player.chosenPerk;
            auto index = valueFind(player.availablePerks, player.chosenPerk);
            if( index != player.availablePerks.size() )
            {
               player.availablePerks.erase(player.availablePerks.begin() + index);
            }
         }
      }
   }

   void updatePerks(RainbowlandGame& game)
   {
      for( auto& player : game.m_players )
      {
         for( auto& perk : player.acquiredPerks )
         {
            if( game.m_perkDatabase[perk.type].updateLogic )
            {
               game.m_perkDatabase[perk.type].updateLogic(perk, player, game);
            }
         }
      }
   }

   void activateAbility(Player& player, RainbowlandGame& game)
   {
      switch( player.ability )
      {
         case AT_HealingCloud:
         {
            activateHealingCloud(player, game);
         } break;
         case AT_DefenseMatrix:
         {
            activateDefenseMatrix(player, game);
         } break;
         case AT_TimeCapsule:
         {
            activateTimeCapsule(player, game);
         } break;
         case AT_Turret:
         {
            activateTurret(player, game);
         } break;
      }
   }

   uint32_t abilityTimeLeft(RainbowlandGame& game, AbilityType ability)
   {
      switch( ability )
      {
         case AT_HealingCloud:
         {
            return healingCloudTimeLeft(game);
         } break;
         case AT_DefenseMatrix:
         {
            return defenseMatrixTimeLeft(game);
         } break;
         case AT_TimeCapsule:
         {
            return timeCapsuleTimeLeft(game);
         } break;
         case AT_Turret:
         {
            return turretTimeLeft(game);
         } break;
      }

      return 0;
   }

   float abilityPercentCharged(RainbowlandGame& game, AbilityType ability)
   {
      switch( ability )
      {
         case AT_HealingCloud:
         {
            return game.m_healingCloud.timer.getPercentDone();
         } break;
         case AT_DefenseMatrix:
         {
            return game.m_defenseMatrix.timer.getPercentDone();
         } break;
         case AT_TimeCapsule:
         {
            return game.m_timeCapsule.timer.getPercentDone();
         } break;
         case AT_Turret:
         {
            return game.m_turret.timer.getPercentDone();
         } break;
      }

      return 0;
   }

   bool isAbilityActive(RainbowlandGame& game, AbilityType ability)
   {
      switch( ability )
      {
         case AT_HealingCloud:
         {
            return game.m_healingCloud.active;
         } break;
         case AT_DefenseMatrix:
         {
            return game.m_defenseMatrix.active;
         } break;
         case AT_TimeCapsule:
         {
            return game.m_timeCapsule.active;
         } break;
         case AT_Turret:
         {
            return game.m_turret.active;
         } break;
      }

      return false;
   }

   void activateDefenseMatrix(Player& player, RainbowlandGame& game)
   {
      if( !game.m_defenseMatrix.active && game.m_defenseMatrix.timer.hasElapsed() )
      {
         game.m_defenseMatrix.active = true;
         game.m_defenseMatrix.area.center = player.transform.position;
         game.m_defenseMatrix.timer.setDurationMicros(secondsToMicros(game.m_defenseMatrix.durationSeconds));
         game.m_defenseMatrix.timer.reset();
      }
   }

   void updateDefenseMatrix(RainbowlandGame& game)
   {
      game.m_defenseMatrix.timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      if( game.m_defenseMatrix.active )
      {
         auto area = game.m_defenseMatrix.area;

         auto cell = calculateCellCoords(game.m_monsterGrid, area.center);
         auto monsters = collectMonstersInArea(game.m_monsterGrid, cell, {2, 2});

         //push monsters out
         for( auto* monster : monsters )
         {
            Circle monstaCollider{monster->transform.position,
               monster->collisionData_hitbox.radius*monster->transform.scale.x};
            if( isCircleTouchingCircle(area, monstaCollider) )
            {
               auto dir = Vec2f::normalize(monstaCollider.center - area.center);
               monster->transform.position = area.center + dir*(area.radius + monstaCollider.radius);
            }
         }
         //check if over
         if( game.m_defenseMatrix.timer.hasElapsed() )
         {
            game.m_defenseMatrix.timer.reset();
            game.m_defenseMatrix.timer.setDurationMicros(secondsToMicros(game.m_defenseMatrix.cooldownSeconds));
            game.m_defenseMatrix.active = false;
         }
      }
   }

   void activateTimeCapsule(Player& player, RainbowlandGame& game)
   {
      if( !game.m_timeCapsule.active && game.m_timeCapsule.timer.hasElapsed() )
      {
         game.m_timeCapsule.active = true;
         game.m_timeCapsule.area.center = player.transform.position;
         game.m_timeCapsule.timer.setDurationMicros(secondsToMicros(game.m_timeCapsule.durationSeconds));
         game.m_timeCapsule.timer.reset();
      }
   }

   void updateTimeCapsule(RainbowlandGame& game)
   {
      game.m_timeCapsule.timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      if( game.m_timeCapsule.active )
      {
         auto area = game.m_timeCapsule.area;
         std::vector<Monster*> monstersAffected;
         auto cell = calculateCellCoords(game.m_monsterGrid, area.center);
         auto monsters = collectMonstersInArea(game.m_monsterGrid, cell, {2, 2});

         for( auto* monster : monsters )
         {
            Circle monstaCollider{monster->transform.position,
               monster->collisionData_hitbox.radius*monster->transform.scale.x};
            if( isCircleTouchingCircle(area, monstaCollider) )
            {
               auto distance2 = Vec2f::length2(monstaCollider.center - area.center);
               auto radius2 = area.radius*area.radius;
               if( distance2 - radius2 < 0 )
               {
                  monstersAffected.emplace_back(monster);
               }
            }
         }
         for( auto* monster : monstersAffected )
         {
            auto distance2 = Vec2f::length2(monster->transform.position - area.center);
            auto radius2 = area.radius*area.radius;
            auto diff = radius2 - distance2;
            float scale = 1.0f - diff / radius2;
            clamp(0.2f, 1.0f, scale);
            monster->objectTimer.setTimeScale(scale);
         }

         //check if over
         if( game.m_timeCapsule.timer.hasElapsed() )
         {
            game.m_timeCapsule.timer.reset();
            game.m_timeCapsule.timer.setDurationMicros(secondsToMicros(game.m_timeCapsule.cooldownSeconds));
            game.m_timeCapsule.active = false;
            for( auto* monster : monstersAffected )
            {
               monster->objectTimer.setTimeScale(1);
            }
         }
      }
   }

   void activateHealingCloud(Player& player, RainbowlandGame& game)
   {
      if( !game.m_healingCloud.active && game.m_healingCloud.timer.hasElapsed() )
      {
         game.m_healingCloud.active = true;
         game.m_healingCloud.area.center = player.transform.position;
         game.m_healingCloud.timer.setDurationMicros(secondsToMicros(game.m_healingCloud.durationSeconds));
         game.m_healingCloud.timer.reset();
         game.m_healingCloud.healTimer.hardReset();
      }
   }

   void updateHealingCloud(RainbowlandGame& game)
   {
      game.m_healingCloud.timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      if( game.m_healingCloud.active )
      {
         auto area = game.m_healingCloud.area;

         game.m_healingCloud.healTimer.updateBy(game.m_gameplayTimer.getDeltaMicros());
         if( game.m_healingCloud.healTimer.hasElapsed() )
         {
            game.m_healingCloud.healTimer.period();

            for( auto& player : game.m_players )
            {
               Circle circleCollider{player.transform.position,
                  player.collisionData.radius*player.transform.scale.x};
               if( isCircleTouchingCircle(area, circleCollider) )
               {
                  player.health += game.m_healingCloud.healStrength;
                  clamp(0, (int32_t)player.maxHealth, player.health);
               }
            }
         }

         //check if over
         if( game.m_healingCloud.timer.hasElapsed() )
         {
            game.m_healingCloud.timer.reset();
            game.m_healingCloud.timer.setDurationMicros(secondsToMicros(game.m_healingCloud.cooldownSeconds));
            game.m_healingCloud.active = false;
         }
      }
   }

   void activateTurret(Player& player, RainbowlandGame& game)
   {
      if( !game.m_turret.active && game.m_turret.timer.hasElapsed() )
      {
         game.m_turret.active = true;
         game.m_turret.area.center = player.transform.position;
         game.m_turret.timer.setDurationMicros(secondsToMicros(game.m_turret.durationSeconds));
         game.m_turret.timer.reset();
         game.m_turret.weapon = player.currentWeapon;
         game.m_turret.weaponTimer.setPeriodMicros(game.m_turret.weapon.fireDelay);
         game.m_turret.weaponTimer.hardReset();
         game.m_turret.killCount = 0;
      }
   }

   void updateTurret(RainbowlandGame& game)
   {
      auto& turret = game.m_turret;
      turret.timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      if( turret.active )
      {
         auto area = game.m_turret.area;
         auto cell = calculateCellCoords(game.m_monsterGrid, area.center);
         auto monsters = collectMonstersInArea(game.m_monsterGrid, cell, {1, 1});
         //push everyone out
         for( auto& monster : monsters )
         {
            Circle monstaCollider{monster->transform.position,
               monster->collisionData_hitbox.radius*monster->transform.scale.x};
            if( isCircleTouchingCircle(area, monstaCollider) )
            {
               auto dir = Vec2f::normalize(monstaCollider.center - area.center);
               monster->transform.position = area.center + dir*(area.radius + monstaCollider.radius);
            }
         }
         for( auto& player : game.m_players )
         {
            Circle monstaCollider{player.transform.position, player.collisionData.radius*player.transform.scale.x};
            if( isCircleTouchingCircle(area, monstaCollider) )
            {
               auto dir = Vec2f::normalize(monstaCollider.center - area.center);
               player.transform.position = area.center + dir*(area.radius + monstaCollider.radius);
            }
         }

         //seek target
         if( game.m_monsters.size() > 0 )
         {
            auto randIndex = game.m_randomGenerator.randInt(0, game.m_monsters.size() - 1);
            turret.aim = Vec2f::normalize(game.m_monsters[randIndex]->transform.position - turret.area.center);
         }
         else
         {
            turret.aim.set(game.m_randomGenerator.randFloat(), game.m_randomGenerator.randFloat());
         }

         //fire weapon
         turret.weaponTimer.updateBy(turret.timer.getTimer().getDeltaMicros());
         auto& w = turret.weapon;
         if( turret.weaponTimer.hasElapsed() )
         {
            turret.weaponTimer.period();

            auto aim = Vec2f::normalize(turret.aim);
            switch( w.type )
            {
               case RPG:
               {
                  generateRocket(game.m_rockets, turret.area.center, aim, w.damage, &game.m_turret);
               } break;

               case Pistol:
               case Shotgun:
               case Uzi:
               case Sniper:
               {
                  generateBullets(game.m_bullets, game.m_randomGenerator, w.bulletsPerShot, w.spread,
                                  turret.area.center, aim, w.damage, w.bulletPierce, &game.m_turret);
               } break;
            }
         }

         //check if over
         if( game.m_turret.timer.hasElapsed() )
         {
            game.m_turret.timer.reset();
            game.m_turret.timer.setDurationMicros(secondsToMicros(game.m_turret.cooldownSeconds));
            game.m_turret.active = false;
         }
      }
   }

   uint32_t defenseMatrixTimeLeft(RainbowlandGame& game)
   {
      auto timeLeft = game.m_defenseMatrix.timer.getRemainingMicros();
      auto displayTime = static_cast<uint32_t>(microsToSeconds(timeLeft));
      return displayTime;
   }

   uint32_t healingCloudTimeLeft(RainbowlandGame& game)
   {
      auto  timeLeft = game.m_healingCloud.timer.getRemainingMicros();
      auto displayTime = static_cast<uint32_t>(microsToSeconds(timeLeft));
      return displayTime;
   }

   uint32_t timeCapsuleTimeLeft(RainbowlandGame& game)
   {
      auto timeLeft = game.m_timeCapsule.timer.getRemainingMicros();
      auto displayTime = static_cast<uint32_t>(microsToSeconds(timeLeft));
      return displayTime;
   }

   uint32_t turretTimeLeft(RainbowlandGame& game)
   {
      auto  timeLeft = game.m_turret.timer.getRemainingMicros();
      auto displayTime = static_cast<uint32_t>(microsToSeconds(timeLeft));
      return displayTime;
   }

   void generateSplatter(VKillLocations locations, RainbowlandGame& game)
   {
      Random gen(Clock::getRealTimeMicros());
      for( auto loc : locations )
      {
         loc.transform.position.y = -loc.transform.position.y;
         //(-halfSize, halfSize) -> (0, 2*halfSize)
         loc.transform.position += game.m_playingField.halfSize();
         //(0, 2*halfSize) -> (0, 1)
         loc.transform.position /= (2.0f * game.m_playingField.halfSize());
         //(0, 1) -> (0, windowSize)
         loc.transform.position *= {(float)game.m_window->getSizeX(), (float)game.m_window->getSizeY()};
         //(0, windowSize) -> worldPos
         loc.transform.position = game.m_graphicsSystem.screenToWorld(loc.transform.position, game.m_camera);
         float scale{gen.randFloat()*0.4f + 1.8f};
         float rotation = gen.randFloat() * 360;
         uint32_t splatterIndex;
         switch( loc.monster )
         {
            case Flower:
            {
               splatterIndex = game.m_imageStartIndex_flower + 6;
               scale = loc.transform.scale.x;
               rotation = loc.transform.rotation;
            } break;
            case Butterfly:
            {
               splatterIndex = gen.randInt(game.m_imageStartIndex_splatter, game.m_imageStartIndex_butterfly - 1);
            } break;
            case Ladybug:
            {
               splatterIndex = game.m_imageStartIndex_ladybug + 6;
               scale = loc.transform.scale.x;
               rotation = loc.transform.rotation;
            } break;
            default:
            {
               splatterIndex = 0;
            } break;
         }
         loc.transform.scale.set(scale, scale);
         loc.transform.rotation = rotation;
         Color c{};
         BloodSplatter bs{loc.transform, c, splatterIndex};
         game.m_splatters.emplace_back(bs);
      }
   }

   Vec2f steer_seek(Vec2f position, float maxSpeed, Vec2f targetLocation)
   {
      Vec2f desiredVelocity = Vec2f::normalize(targetLocation - position)*maxSpeed;
      return desiredVelocity;
   }

   Vec2f steer_flee(Vec2f position, float maxSpeed, Vec2f targetLocation)
   {
      Vec2f desiredVelocity = Vec2f::normalize(position - targetLocation)*maxSpeed;
      return desiredVelocity;
   }

   Vec2f steer_arrive(Vec2f position, float maxSpeed, Vec2f targetLocation)
   {
      auto toTarget = targetLocation - position;
      auto distance = Vec2f::length(toTarget);

      if( distance > 0.0f )
      {
         float decelerationTweaker = 0.4f;
         auto speed = distance / decelerationTweaker;
         speed = std::min(speed, maxSpeed);

         auto desiredVelocity = toTarget*speed / distance;
         return desiredVelocity;
      }
      return{0, 0};
   }

   Vec2f steer_pursuit(Vec2f position, Vec2f heading, float maxSpeed, Vec2f targetPosition, Vec2f targetHeading, float targetMaxSpeed)
   {
      auto toEvader = targetPosition - position;
      auto relativeHeading = Vec2f::dotProduct(heading, targetHeading);
      if( Vec2f::dotProduct(toEvader, heading) > 0 &&
         relativeHeading < -0.95f )
      {
         return steer_arrive(position, maxSpeed, targetPosition);
      }

      auto lookAheadTime = Vec2f::length(toEvader) / (maxSpeed + targetMaxSpeed);
      return steer_arrive(position, maxSpeed, (targetPosition + lookAheadTime*(targetHeading*targetMaxSpeed)));
   }

   Vec2f steer_wander(Monster& monster, RainbowlandGame& game)
   {
      (void)monster;
      Vec2f random = Vec2f{game.m_randomGenerator.randFloat() * 2 - 1,
         game.m_randomGenerator.randFloat() * 2 - 1};
      return random;
      /*
      random = Vec2f::normalize(random);

      random += Vec2f::normalize(monster.direction) * 1;

      auto newDir = Vec2f::normalize(random - monster.transform.position);

      return newDir;*/
   }
}
