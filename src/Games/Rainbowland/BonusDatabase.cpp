//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/BonusDatabase.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Rainbowland.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	void initBonusDatabase(VBonuses& bonusDb)
	{
		bonusDb =
		{
			{
				BonusType::Heal, "Heal", 0,
				[](Player& player, RainbowlandGame& game)
				{
					player.health += 20;
					clamp(0, 100, player.health);
				},
					[](Player& player, RainbowlandGame& game) {}
			},

			{
				BonusType::IncreasedRateOfFire, "Shooter", Time::secondsToMicros(5),
				[](Player& player, RainbowlandGame& game)
				{
					auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return IncreasedRateOfFire == e.type; });
					if(index == player.bonuses.size())
					{
						player.bonuses.emplace_back();
						player.bonuses[index].duration = 0;
						player.bonuses[index].timer.reset();
						player.bonuses[index].type = IncreasedRateOfFire;
						player.rateOfFireMultiplier *= 0.5f;
						player.reloadMultiplier *= 0.5f;
						player.currentWeapon.fireDelay = static_cast<uint64_t>(0.5f*static_cast<float>(player.currentWeapon.fireDelay));
						player.currentWeapon.reloadDelay = static_cast<uint64_t>(0.5f*static_cast<float>(player.currentWeapon.reloadDelay));
					}
					player.bonuses[index].duration += game.m_bonusDatabase[IncreasedRateOfFire].durationMicros;
				},
					[](Player& player, RainbowlandGame& game)
				{
					player.rateOfFireMultiplier *= 2;
					player.reloadMultiplier *= 2;
					player.currentWeapon.fireDelay = static_cast<uint64_t>(2 * static_cast<float>(player.currentWeapon.fireDelay));
					player.currentWeapon.reloadDelay = static_cast<uint64_t>(2 * static_cast<float>(player.currentWeapon.reloadDelay));
					auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return IncreasedRateOfFire == e.type; });
					player.bonuses[index] = player.bonuses.back();
					player.bonuses.pop_back();
				}
			},

			{
				BonusType::IncreasedMovementSpeed, "Speedy", Time::secondsToMicros(5), 
				[](Player& player, RainbowlandGame& game)
				{
					auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return IncreasedMovementSpeed == e.type; });
					if(index == player.bonuses.size())
					{
						player.bonuses.emplace_back();
						player.bonuses[index].duration = 0;
						player.bonuses[index].timer.reset();
						player.bonuses[index].type = IncreasedMovementSpeed;
						player.maxVelocity *= 2;
					}
					player.bonuses[index].duration += game.m_bonusDatabase[IncreasedMovementSpeed].durationMicros;
				},
					[](Player& player, RainbowlandGame& game)
				{
					player.maxVelocity *= 0.5f;
					auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return IncreasedMovementSpeed == e.type; });
					player.bonuses[index] = player.bonuses.back();
					player.bonuses.pop_back();
				}
			},

			{
				BonusType::SlowTime, "Slow time", Time::secondsToMicros(8),
				[](Player& player, RainbowlandGame& game)
				{
					auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return SlowTime == e.type; });
					if(index == player.bonuses.size())
					{
						player.bonuses.emplace_back();
						player.bonuses[index].duration = 0;
						player.bonuses[index].timer.reset();
						player.bonuses[index].type = SlowTime;
						game.m_gameplayTimer.setTimeScale(game.m_gameplayTimer.getTimeScale()*0.4);
					}
					player.bonuses[index].duration += game.m_bonusDatabase[SlowTime].durationMicros;
				},
					[](Player& player, RainbowlandGame& game)
				{
					game.m_gameplayTimer.setTimeScale(game.m_gameplayTimer.getTimeScale()/0.4);
					auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return SlowTime == e.type; });
					player.bonuses[index] = player.bonuses.back();
					player.bonuses.pop_back();
				}
			},

			{
				BonusType::Weapon_Pistol, "Pistol", 0,
				[](Player& player, RainbowlandGame& game)
				{
					selectWeapon(player, Pistol, game.m_weaponDatabase);
				},
					[](Player& player, RainbowlandGame& game) {}
			},

			{
				BonusType::Weapon_Shotgun, "Shotgun", 0,
				[](Player& player, RainbowlandGame& game)
				{
					selectWeapon(player, Shotgun, game.m_weaponDatabase);
				},
					[](Player& player, RainbowlandGame& game) {}
			},

			{
				BonusType::Weapon_Uzi, "Uzi", 0,
				[](Player& player, RainbowlandGame& game)
				{
					selectWeapon(player, Uzi, game.m_weaponDatabase);
				},
					[](Player& player, RainbowlandGame& game) {}
			},

			{
				BonusType::Weapon_Sniper, "Sniper", 0,
					[](Player& player, RainbowlandGame& game)
				{
					selectWeapon(player, Sniper, game.m_weaponDatabase);
				},
					[](Player& player, RainbowlandGame& game) {}
			}
		};

		std::sort(bonusDb.begin(), bonusDb.end(),
				  [](const Bonus& l, const Bonus& r)
		{
			return l.type < r.type;
		});
	}
}
