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
      player.aimDirection = Vec2::normalize(player.aim);
      player.aimDistance = 0;
      player.maxAimDistance = 6;
      player.minAimDistance = 1.5;
      //player.currentWeapon set in selectWeapon
      player.rateOfFireMultiplier = 1;
      player.reloadMultiplier = 1;
      player.ammoMultiplier = 1;
      player.extraBullets = 0;
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
            player.ability = activateBlink;
            player.skillCooldown = &game.m_blink.cooldownSeconds;
            game.m_blinkPlaying = true;
         } break;
         case 1:
         {
            player.color.set(1, 1, dimm);
            player.ability = activateDefenseMatrix;
            player.skillCooldown = &game.m_defenseMatrix.cooldownSeconds;
            game.m_defenseMatrixPlaying = true;
         } break;
         case 2:
         {
            player.color.set(dimm, 1, dimm);
            player.ability = activateTurret;
            player.skillCooldown = &game.m_turret.cooldownSeconds;
            game.m_turretPlaying = true;
         } break;
         case 3:
         {
            player.color.set(dimm, dimm, 1);
            player.ability = activateTimeCapsule;
            player.skillCooldown = &game.m_timeCapsule.cooldownSeconds;
            game.m_timeCapsulePlaying = true;
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

               Vec2 targetVector;
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

               player.targetDirection = Vec2::normalize(targetVector);
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

         currentDeg += wra * 4 * player.objectTimer.getDeltaTime();
         currentRad = Deg2Rad(currentDeg);

         player.direction.x = std::cosf(currentRad);
         player.direction.y = std::sinf(currentRad);

         auto velocity = player.direction*player.currentSpeed;
         player.transform.position += (player.objectTimer.getDeltaTime()*velocity);
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
         aimDir = Vec2::normalize(aimDir);
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
      if( game.m_experience >= game.m_experienceForNextLevel )
      {
         game.m_experienceForNextLevel += game.m_experienceIncrement;
         ++game.m_level;
         enterPerkMode(game);
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
         Vec2 averagePos;
         for( auto& player : game.m_players )
         {
            averagePos += player.transform.position;
         }
         averagePos /= (float)game.m_players.size();
         auto pos = game.m_camera.getPosition();
         Vec2 pos2{pos.x, pos.y};
         clamp(-game.m_cameraBounds.x, game.m_cameraBounds.x, averagePos.x);
         clamp(-game.m_cameraBounds.y, game.m_cameraBounds.y, averagePos.y);
         auto diff = averagePos - pos2;
         if( Vec2::length(diff) > 0 )
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
      if( (int64_t)game.m_difficultyTimer.getCurrentMicros() >= Time::milisToMicros(game.m_difficultyIncrementMilis) )
      {
         game.m_difficultyTimer.updateBy(-Time::milisToMicros(game.m_difficultyIncrementMilis));
         game.m_difficulty += 0.1f;
      }
      if( game.m_killCounter >= 100 )
      {
         game.m_difficulty += 0.5f;
         game.m_killCounter -= 100;
      }
   }

   void enableBonus(BonusType bonus, Vec2 pickupPosition, Player& player, RainbowlandGame& game)
   {
      game.m_bonusDatabase[bonus].acquireLogic(pickupPosition, player, game);
   }

   void disableBonus(BonusType bonus, RainbowlandGame& game)
   {
      game.m_bonusDatabase[bonus].timeoutLogic(game);
   }

   void updateBonuses(RainbowlandGame& game)
   {
      for( uint32_t b = 0; b < game.m_activeBonuses.size(); )
      {
         auto& bonus = game.m_activeBonuses[b];
         bonus.timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
         if( bonus.timer.getCurrentMicros() > bonus.duration )
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
      if( game.m_pickups.size() >= 5 )
      {
         return;
      }

      for( auto& loc : killLocations )
      {
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
            if( std::any_of(game.m_pickups.begin(), game.m_pickups.end(),
               [](Pickup& p) { return p.weapon != WeaponTypeCount; }) )
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

   void placePickup(RainbowlandGame& game, Vec2 location, BonusType bonus, WeaponType weapon)
   {
      game.m_pickups.emplace_back();
      game.m_pickups.back().objectTimer.reset();
      game.m_pickups.back().transform.position = location;
      game.m_pickups.back().transform.scale.set(1.1f, 1.1f);
      game.m_pickups.back().collisionData.set(0, 0, 1);
      game.m_pickups.back().bonus = bonus;
      game.m_pickups.back().weapon = weapon;
      game.m_pickups.back().duration = Time::secondsToMicros(15);

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
         if( game.m_pickups[b].objectTimer.getCurrentMicros() > game.m_pickups[b].duration )
         {
            game.m_pickups[b] = game.m_pickups.back();
            game.m_pickups.pop_back();
         }
         else
         {
            auto duration = game.m_pickups[b].duration;
            auto diff = duration - game.m_pickups[b].objectTimer.getCurrentMicros();
            game.m_pickups[b].color.a = (float)diff / Time::secondsToMicros(2);
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
      player.weaponTimer.updateBy(Time::secondsToMicros(50));
   }

   void calculateWeaponBonuses(Player& player, const VWeapons& weaponDb)
   {
      const auto& dbw = weaponDb[player.currentWeapon.type];
      player.currentWeapon.damage = player.bonusDamage + dbw.damage;
      player.currentWeapon.fireDelay = static_cast<uint64_t>(player.rateOfFireMultiplier*static_cast<float>(dbw.fireDelay));
      player.currentWeapon.maxAmmo = static_cast<uint32_t>(player.ammoMultiplier*static_cast<float>(dbw.maxAmmo));
      player.currentWeapon.reloadDelay = static_cast<uint64_t>(player.reloadMultiplier*static_cast<float>(dbw.reloadDelay));
   }

   void fireWeapons(RainbowlandGame& game)
   {
      for( auto& player : game.m_players )
      {
         auto& w = player.currentWeapon;
         if( w.ammo > 0 )
         {
            player.weaponTimer.updateBy(player.objectTimer.getDeltaMicros());
            if( player.isShooting && player.weaponTimer.getCurrentMicros() > w.fireDelay )
            {
               player.weaponTimer.reset();
               --w.ammo;
               auto aim = player.aim;
               if( !player.isAimRelative )
               {
                  aim -= player.transform.position;
               }
               aim = Vec2::normalize(aim);
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
                     generateBullets(game.m_bullets, game.m_randomGenerator, w.bulletsPerShot + player.extraBullets,
                                     w.spread, player.transform.position + p, aim, w.damage, w.bulletPierce, &player);
                  } break;
               }

               game.m_audioSystem.playSfx(game.m_shootSfx);
            }
         }
         else
         {
            if( player.weaponTimer.getCurrentMicros() == 0 )
            {
               game.m_audioSystem.playSfx(game.m_reloadSfx);
            }
            player.weaponTimer.updateBy(player.objectTimer.getDeltaMicros());
            if( player.weaponTimer.getCurrentMicros() > w.reloadDelay )
            {
               w.ammo = w.maxAmmo;
               player.weaponTimer.updateBy(Time::secondsToMicros(50));
            }
         }
      }
   }

   void generateBullets(VBullets& bullets, Random& gen, uint32_t count, float spread, Vec2 origin, Vec2 direction, uint32_t damage, bool pierce, void* owner)
   {
      Vec2 targetDistance = direction * 30;
      for( uint32_t i = 0; i < count; ++i )
      {
         Vec2 t{gen.randFloat()*spread * 2 - spread, gen.randFloat()*spread * 2 - spread};
         bullets.emplace_back(Bullet{Time(), origin, origin, origin, Vec2::normalize(targetDistance + t) * 60, 0, damage, owner, pierce});
         bullets.back().objectTimer.reset();
      }
   }

   void generateBlast(VBlasts& blasts, Vec2 location, float startRadius, float endRadius, float duration, int32_t damage, void* owner)
   {
      Blast blast;
      blast.area.center = location;
      blast.area.radius = startRadius;
      blast.maxRadius = endRadius;
      blast.rotation = 0;
      blast.damage = damage;
      blast.duration = Time::secondsToMicros(duration);
      blast.objectTimer.reset();
      blast.owner = owner;
      blasts.emplace_back(blast);
   }

   void generateRocket(VRockets& rockets, Vec2 location, Vec2 direction, uint32_t damage, void* owner)
   {
      Rocket rocky;
      rocky.objectTimer.reset();
      rocky.duration = Time::secondsToMicros(24);
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
         auto displacement = bullets[i].objectTimer.getDeltaTime()*bullets[i].velocity;
         auto travel = Vec2::length(displacement);
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
            auto old = Vec2::normalize(bullets[i].oldPosition - bullets[i].position);
            auto trail = Vec2::normalize(bullets[i].trail - bullets[i].position);
            auto dot = Vec2::dotProduct(old, trail);
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
         auto percent = static_cast<float>(blast.objectTimer.getCurrentMicros());
         percent /= static_cast<float>(blast.duration);
         blast.area.radius = blast.maxRadius*percent;
         blast.rotation += Deg2Rad(5);
      }
   }

   void moveRockets(VRockets& rockets)
   {
      for( auto& rocket : rockets )
      {
         rocket.body.center += rocket.direction*rocket.speed*rocket.objectTimer.getDeltaTime();
      }
   }

   void findBulletHits(const Bullet& bullet, const VMonsters& monsters, std::vector<uint32_t>& outMonsters)
   {
      for( uint32_t m = 0; m < monsters.size(); ++m )
      {
         if( monsters[m].health > 0 )
         {
            Circle bCollider = monsters[m].collisionData_hitbox;
            bCollider.center = monsters[m].transform.position;
            bCollider.radius *= monsters[m].transform.scale.x;
            if( isLineTouchingCircle(bullet.oldPosition, bullet.position, bCollider) )
            {
               outMonsters.push_back(m);
            }
         }
      }
   }

   void findBlastHits(const Blast& blast, const VMonsters& monsters, std::vector<uint32_t>& outMonsters)
   {
      for( uint32_t m = 0; m < monsters.size(); ++m )
      {
         if( monsters[m].health > 0 )
         {
            Circle bCollider = monsters[m].collisionData_hitbox;
            bCollider.center = monsters[m].transform.position;
            bCollider.radius *= monsters[m].transform.scale.x;
            if( isCircleTouchingCircle(bCollider, blast.area) )
            {
               outMonsters.push_back(m);
            }
         }
      }
   }

   void findRocketHits(const Rocket& rock, const VMonsters& monsters, std::vector<uint32_t>& outMonsters)
   {
      for( uint32_t m = 0; m < monsters.size(); ++m )
      {
         if( monsters[m].health > 0 )
         {
            Circle bCollider = monsters[m].collisionData_hitbox;
            bCollider.center = monsters[m].transform.position;
            bCollider.radius *= monsters[m].transform.scale.x;
            if( isCircleTouchingCircle(bCollider, rock.body) )
            {
               outMonsters.push_back(m);
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
            std::vector<uint32_t> hitMonsters;
            findBulletHits(bullet, game.m_monsters, hitMonsters);
            if( !hitMonsters.empty() )
            {
               std::sort(hitMonsters.begin(), hitMonsters.end(), [&](uint32_t l, uint32_t r)
               {
                  auto ll = Vec2::length(game.m_monsters[l].transform.position - bullet.trail);
                  auto rl = Vec2::length(game.m_monsters[r].transform.position - bullet.trail);
                  return ll < rl;
               });
               for( auto mid : hitMonsters )
               {
                  hurtMonster(game.m_monsters[mid], bullet.damage);
                  if( game.m_monsters[mid].health <= 0 )
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
            std::vector<uint32_t> hitMonsters;
            findBlastHits(blast, game.m_monsters, hitMonsters);
            if( !hitMonsters.empty() )
            {
               for( auto mid : hitMonsters )
               {
                  hurtMonster(game.m_monsters[mid], blast.damage);
                  if( game.m_monsters[mid].health <= 0 )
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
         bool done = rock.objectTimer.getCurrentMicros() > rock.duration;
         if( !done )
         {
            //get monsters inside radius
            std::vector<uint32_t> hitMonsters;
            findRocketHits(rock, game.m_monsters, hitMonsters);
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
         spawner.timer.updateBy(spawner.objectTimer.getDeltaMicros());
         if( spawner.timer.getCurrentMicros() > spawner.spawnCooldown )
         {
            spawner.timer.reset();
            if( game.m_monsters.size() < 450 )
            {
               Vec2 displacement{(game.m_randomGenerator.randFloat() * 2 - 1)*spawner.spawnRadius,
                  (game.m_randomGenerator.randFloat() * 2 - 1)*spawner.spawnRadius};
               generateMonster(game.m_monsters,
                               spawner.transform.position + displacement,
                               game.m_randomGenerator.randInt(0, game.m_players.size() - 1),
                               game);
            }
         }
      }
   }

   void scaleMonsterToDifficulty(Monster& monsta, RainbowlandGame& game)
   {
      monsta.health = static_cast<uint32_t>(static_cast<float>(monsta.health)*game.m_difficulty);
      monsta.maxHealth = monsta.health;
      monsta.damage = static_cast<uint32_t>(static_cast<float>(monsta.damage)*game.m_difficulty);
   }

   void setFlowerSpecificData(Monster& monster)
   {
      monster.collisionData_attack.set(0, 0, 0.6f);
      monster.collisionData_hitbox.set(0, 0, 0.7f);
   }

   void setLadybugSpecificData(Monster& monster)
   {
      monster.collisionData_attack.set(0, 0, 0.7f);
      monster.collisionData_hitbox.set(0, 0, 0.8f);
   }

   void setButterflySpecificData(Monster& monster)
   {
      monster.collisionData_attack.set(0, 0, 0.5f);
      monster.collisionData_hitbox.set(0, 0, 0.7f);
   }

   void generateMonster(VMonsters& monsters, Vec2 position, uint32_t target, RainbowlandGame& game)
   {
      monsters.emplace_back();
      auto& monster = monsters.back();

      monster.objectTimer.reset();
      monster.maxSpeed = 1 + game.m_randomGenerator.randFloat() * 1.5f;
      monster.turnSpeed = 1 + monster.maxSpeed;
      monster.direction.set(1, 0);
      monster.transform.position = position;
      auto scale = 1.3f - game.m_randomGenerator.randFloat()*0.6f;
      monster.transform.scale.set(scale, scale);
      monster.collisionData_separation.set(0, 0, 0.5f);
      monster.brain.timer.reset();
      monster.brain.updateDelay = Time::secondsToMicros(game.m_randomGenerator.randFloat());
      monster.brain.timer.updateBy(monster.brain.updateDelay);
      monster.brain.targetPlayer = -1;
      monster.brain.targetLocation = position;
      monster.brain.steerLogic = SB_Arrive;
      uint32_t hp = static_cast<uint32_t>(20 * (game.m_randomGenerator.randFloat()*0.4f + 0.8f));
      monster.maxHealth = monster.health = static_cast<uint32_t>(hp);
      monster.attackTimer.updateBy(Time::secondsToMicros(5));
      monster.attackDelay = Time::secondsToMicros(0.4f);
      monster.damage = 1;
      monster.expGain = hp;
      monster.color.set(1, 1, 1);
      if( game.m_randomGenerator.randInt(0, 100 * 100) < 25 * 100 )
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
   }

   void runMonsterAI(RainbowlandGame& game)
   {
      for( auto& monster : game.m_monsters )
      {
         monster.brain.timer.updateBy(monster.objectTimer.getDeltaMicros());
         if( monster.brain.timer.getCurrentMicros() >= monster.brain.updateDelay )
         {
            monster.brain.timer.reset();
            //monster brain does work here
            //decide which player to target
            if( game.m_players.size() > 0 )
            {
               auto playerId = monster.brain.targetPlayer;
               auto playerIndex = filterFind(game.m_players, [=](const Player& p) { return p.id == playerId; });
               if( playerIndex == game.m_players.size() || monster.brain.targetPlayer >= game.m_players.size() )
               {
                  playerIndex = game.m_randomGenerator.randInt(0, game.m_players.size() - 1);
                  monster.brain.targetPlayer = game.m_players[playerIndex].id;
                  monster.brain.steerLogic = SB_Pursuit;
               }
               monster.brain.targetLocation = game.m_players[playerIndex].transform.position;
            }
            else
            {
               monster.brain.steerLogic = SB_Wander;
            }
         }
      }
   }

   void separateMonsters(Monster& monsta, VMonsters& monsters)
   {
      for( auto& monster : monsters )
      {
         if( &monsta == &monster ) continue;

         auto distanceVec = monsta.transform.position - monster.transform.position;
         auto distanceLen = Vec2::length(distanceVec);

         auto overlap = monsta.collisionData_separation.radius*monsta.transform.scale.x +
            monster.collisionData_separation.radius*monster.transform.scale.x - distanceLen;
         if( overlap > 0 )
         {
            monsta.transform.position += (overlap*distanceVec / distanceLen);
         }
      }
   }

   void moveMonsters(RainbowlandGame& game)
   {
      for( auto& monster : game.m_monsters )
      {
         switch( monster.brain.steerLogic )
         {
            case SB_Arrive:
            {
               auto n = Vec2::length(steer_arrive(monster.transform.position, monster.maxSpeed, monster.brain.targetLocation));
               monster.transform.position += monster.direction*n*monster.objectTimer.getDeltaTime();
            } break;

            case SB_Pursuit:
            {
               auto playerId = monster.brain.targetPlayer;
               auto playerIndex = filterFind(game.m_players, [=](const Player& p) { return p.id == playerId; });
               auto& player = game.m_players[playerIndex];
               auto n = Vec2::length(steer_pursuit(monster.transform.position, monster.direction, monster.maxSpeed,
                  player.transform.position, player.direction, player.maxSpeed));
               monster.transform.position += monster.direction*n*monster.objectTimer.getDeltaTime();
            } break;
         }
         separateMonsters(monster, game.m_monsters);
      }
   }

   void orientMonsters(VMonsters& monsters)
   {
      for( auto& monster : monsters )
      {
         float direction = std::atan2f(monster.direction.y, monster.direction.x);
         auto targetDirection = Vec2::normalize(monster.brain.targetLocation - monster.transform.position);
         float target = std::atan2f(targetDirection.y, targetDirection.x);
         direction = Rad2Deg(direction);
         target = Rad2Deg(target);

         auto wra = target - direction;
         if( wra > 180.0f ) wra -= 360.0f;
         if( wra < -180.0f ) wra += 360.0f;

         auto newDir = direction + wra*monster.turnSpeed*monster.objectTimer.getDeltaTime();
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
         for( auto& monster : game.m_monsters )
         {
            auto mCollider = monster.collisionData_attack;
            mCollider.center = monster.transform.position;
            mCollider.radius *= monster.transform.scale.x;
            if( isCircleTouchingCircle(mCollider, pCollider) )
            {
               monster.attackTimer.updateBy(monster.objectTimer.getDeltaMicros());
               if( monster.attackTimer.getCurrentMicros() >= monster.attackDelay )
               {
                  if( game.m_randomGenerator.randInt(0, 100 * 100) >= (int32_t)player.dodgeChance * 100 )
                  {
                     player.health -= monster.damage;
                  }
                  monster.attackTimer.reset();
               }
            }
            else
            {
               monster.attackTimer.updateBy(Time::secondsToMicros(1));
            }
         }
      }
   }

   void killMonsters(RainbowlandGame& game, VKillLocations& killLocations, bool grantExp)
   {
      for( uint32_t m = 0; m < game.m_monsters.size(); )
      {
         if( game.m_monsters[m].health <= 0 )
         {
            if( grantExp )
            {
               grantExperience(game.m_monsters[m].expGain, game);
            }
            killLocations.emplace_back(KillLocation{game.m_monsters[m].transform, game.m_monsters[m].type});
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
      game.m_nextGameState = RainbowlandGame::GS_SessionPerkMenu;
      for( auto& player : game.m_players )
      {
         player.directionActive[Up] =
            player.directionActive[Down] =
            player.directionActive[Left] =
            player.directionActive[Right] = false;
         player.currentSpeed = 0;
         player.targetDirection = player.direction;
      }
   }

   void generatePerks(VPlayers& players, const VPerks& perkDb)
   {
      Random gen{Time::getRealTimeMicros()};

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
               if( perkDb[perks[perkIndex]].dependencyCheck(player) )
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
         perk.acquireLogic(player, game);

         player.selectablePerks.clear();
         if( !perk.repeatable )
         {
            player.acquiredPerks.emplace_back(player.chosenPerk);
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
         for( auto perk : player.acquiredPerks )
         {
            game.m_perkDatabase[perk].updateLogic(player, game);
         }
      }
   }

   void activateDefenseMatrix(Player& player, RainbowlandGame& game)
   {
      if( !game.m_defenseMatrix.active && game.m_defenseMatrix.timer.getCurrentMicros() >= game.m_defenseMatrix.updateDelay )
      {
         game.m_defenseMatrix.active = true;
         game.m_defenseMatrix.area.center = player.transform.position;
         game.m_defenseMatrix.updateDelay = Time::secondsToMicros(game.m_defenseMatrix.durationSeconds);
         game.m_defenseMatrix.timer.reset();
      }
   }

   void updateDefenseMatrix(RainbowlandGame& game)
   {
      game.m_defenseMatrix.timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      if( game.m_defenseMatrix.active )
      {
         auto area = game.m_defenseMatrix.area;
         //push monsters out
         for( auto& monster : game.m_monsters )
         {
            Circle monstaCollider{monster.transform.position, monster.collisionData_hitbox.radius*monster.transform.scale.x};
            if( isCircleTouchingCircle(area, monstaCollider) )
            {
               auto dir = Vec2::normalize(monstaCollider.center - area.center);
               monster.transform.position = area.center + dir*(area.radius + monstaCollider.radius);
            }
         }
         //check if over
         if( game.m_defenseMatrix.timer.getCurrentMicros() >= game.m_defenseMatrix.updateDelay )
         {
            game.m_defenseMatrix.timer.reset();
            game.m_defenseMatrix.updateDelay = Time::secondsToMicros(game.m_defenseMatrix.cooldownSeconds);
            game.m_defenseMatrix.active = false;
         }
      }
   }

   void activateTimeCapsule(Player& player, RainbowlandGame& game)
   {
      if( !game.m_timeCapsule.active && game.m_timeCapsule.timer.getCurrentMicros() >= game.m_timeCapsule.updateDelay )
      {
         game.m_timeCapsule.active = true;
         game.m_timeCapsule.area.center = player.transform.position;
         game.m_timeCapsule.updateDelay = Time::secondsToMicros(game.m_timeCapsule.durationSeconds);
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
         for( auto& monster : game.m_monsters )
         {
            Circle monstaCollider{monster.transform.position, monster.collisionData_hitbox.radius*monster.transform.scale.x};
            if( isCircleTouchingCircle(area, monstaCollider) )
            {
               auto distance2 = Vec2::length2(monstaCollider.center - area.center);
               auto radius2 = area.radius*area.radius;
               if( distance2 - radius2 < 0 )
               {
                  monstersAffected.emplace_back(&monster);
               }
            }
         }
         for( auto* monster : monstersAffected )
         {
            auto distance2 = Vec2::length2(monster->transform.position - area.center);
            auto radius2 = area.radius*area.radius;
            auto diff = radius2 - distance2;
            double scale = 1.0 - (double)diff / (double)radius2;
            clamp(0.2, 1.0, scale);
            monster->objectTimer.setTimeScale(scale);
         }
         //check if over
         if( game.m_timeCapsule.timer.getCurrentMicros() >= game.m_timeCapsule.updateDelay )
         {
            game.m_timeCapsule.timer.reset();
            game.m_timeCapsule.updateDelay = Time::secondsToMicros(game.m_timeCapsule.cooldownSeconds);
            game.m_timeCapsule.active = false;
            for( auto* monster : monstersAffected )
            {
               monster->objectTimer.setTimeScale(Time::NORMAL_TIME);
            }
         }
      }
   }

   void activateBlink(Player& player, RainbowlandGame& game)
   {
      if( !game.m_blink.active && game.m_blink.timer.getCurrentMicros() >= game.m_blink.updateDelay )
      {
         game.m_blink.target = Vec2::normalize(player.aim)*15;
         game.m_blink.active = true;
         game.m_blink.area.center = player.transform.position;
         game.m_blink.updateDelay = Time::secondsToMicros(game.m_blink.durationSeconds);
         game.m_blink.timer.reset();
      }
   }

   void updateBlink(RainbowlandGame& game)
   {
      game.m_blink.timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
      if( game.m_blink.active )
      {
         auto area = game.m_blink.area;

         auto distanceVector = game.m_blink.target - area.center;
         
         for( auto& p : game.m_players )
         {
            if( isPointInsideCircle(p.transform.position, game.m_blink.area) )
            {
               p.transform.position += distanceVector;
            }
         }

         //check if over
         if( game.m_blink.timer.getCurrentMicros() >= game.m_blink.updateDelay )
         {
            game.m_blink.timer.reset();
            game.m_blink.updateDelay = Time::secondsToMicros(game.m_blink.cooldownSeconds);
            game.m_blink.active = false;
         }
      }
   }

   void activateTurret(Player& player, RainbowlandGame& game)
   {
      if( !game.m_turret.active && game.m_turret.timer.getCurrentMicros() >= game.m_turret.updateDelay )
      {
         game.m_turret.active = true;
         game.m_turret.area.center = player.transform.position;
         game.m_turret.updateDelay = Time::secondsToMicros(game.m_turret.durationSeconds);
         game.m_turret.timer.reset();
         game.m_turret.weapon = player.currentWeapon;
         game.m_turret.weaponTimer.reset();
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
         //push everyone out
         for( auto& monster : game.m_monsters )
         {
            Circle monstaCollider{monster.transform.position, monster.collisionData_hitbox.radius*monster.transform.scale.x};
            if( isCircleTouchingCircle(area, monstaCollider) )
            {
               auto dir = Vec2::normalize(monstaCollider.center - area.center);
               monster.transform.position = area.center + dir*(area.radius + monstaCollider.radius);
            }
         }
         for( auto& monster : game.m_players )
         {
            Circle monstaCollider{monster.transform.position, monster.collisionData.radius*monster.transform.scale.x};
            if( isCircleTouchingCircle(area, monstaCollider) )
            {
               auto dir = Vec2::normalize(monstaCollider.center - area.center);
               monster.transform.position = area.center + dir*(area.radius + monstaCollider.radius);
            }
         }

         //seek target
         if( game.m_monsters.size() > 0 )
         {
            auto randIndex = game.m_randomGenerator.randInt(0, game.m_monsters.size() - 1);
            turret.aim = Vec2::normalize(game.m_monsters[randIndex].transform.position - turret.area.center);
         }
         else
         {
            turret.aim.set(game.m_randomGenerator.randFloat(), game.m_randomGenerator.randFloat());
         }

         //fire weapon
         turret.weaponTimer.updateBy(turret.timer.getDeltaMicros());
         auto& w = turret.weapon;
         if( turret.weaponTimer.getCurrentMicros() >= w.fireDelay )
         {
            turret.weaponTimer.reset();

            auto aim = Vec2::normalize(turret.aim);
            auto p = aim*turret.area.radius;
            generateBullets(game.m_bullets, game.m_randomGenerator, w.bulletsPerShot, w.spread, turret.area.center + p,
                            aim, w.damage, w.bulletPierce, &game.m_turret);
         }

         //check if over
         if( game.m_turret.timer.getCurrentMicros() >= game.m_turret.updateDelay )
         {
            game.m_turret.timer.reset();
            game.m_turret.updateDelay = Time::secondsToMicros(game.m_turret.cooldownSeconds);
            game.m_turret.active = false;
         }
      }
   }

   void generateSplatter(VKillLocations locations, RainbowlandGame& game)
   {
      Random gen(Time::getRealTimeMicros());
      for( auto loc : locations )
      {
         loc.transform.position.y = -loc.transform.position.y;
         //(-halfSize, halfSize) -> (0, 2*halfSize)
         loc.transform.position += game.m_playingField.halfSize();
         //(0, 2*halfSize) -> (0, 1)
         loc.transform.position /= (2 * game.m_playingField.halfSize());
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
         }
         loc.transform.scale.set(scale, scale);
         loc.transform.rotation = rotation;
         Color c{};
         BloodSplatter bs{loc.transform, c, splatterIndex};
         game.m_splatters.emplace_back(bs);
      }
   }

   Vec2 steer_seek(Vec2 position, float maxSpeed, Vec2 targetLocation)
   {
      Vec2 desiredVelocity = Vec2::normalize(targetLocation - position)*maxSpeed;
      return desiredVelocity;
   }

   Vec2 steer_flee(Vec2 position, float maxSpeed, Vec2 targetLocation)
   {
      Vec2 desiredVelocity = Vec2::normalize(position - targetLocation)*maxSpeed;
      return desiredVelocity;
   }

   Vec2 steer_arrive(Vec2 position, float maxSpeed, Vec2 targetLocation)
   {
      auto toTarget = targetLocation - position;
      auto distance = Vec2::length(toTarget);

      if( distance > 0.1f )
      {
         float decelerationTweaker = 0.4f;
         auto speed = distance / decelerationTweaker;
         speed = std::min(speed, maxSpeed);

         auto desiredVelocity = toTarget*speed / distance;
         return desiredVelocity;
      }
      return{0, 0};
   }

   Vec2 steer_pursuit(Vec2 position, Vec2 heading, float maxSpeed, Vec2 targetPosition, Vec2 targetHeading, float targetMaxSpeed)
   {
      auto toEvader = targetPosition - position;
      auto relativeHeading = Vec2::dotProduct(heading, targetHeading);
      if( Vec2::dotProduct(toEvader, heading) > 0 &&
         relativeHeading < -0.95f )
      {
         return steer_arrive(position, maxSpeed, targetPosition);
      }

      auto lookAheadTime = Vec2::length(toEvader) / (maxSpeed + targetMaxSpeed);
      return steer_arrive(position, maxSpeed, (targetPosition + lookAheadTime*(targetHeading*targetMaxSpeed)));
   }

   Vec2 steer_wander()
   {
      return Vec2{};
   }
}
