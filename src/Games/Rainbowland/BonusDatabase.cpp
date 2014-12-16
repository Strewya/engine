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
						calculateWeaponBonuses(player, game.m_weaponDatabase);
					}
					player.bonuses[index].duration += game.m_bonusDatabase[IncreasedRateOfFire].durationMicros;
				},
					[](Player& player, RainbowlandGame& game)
				{
					player.rateOfFireMultiplier *= 2;
					player.reloadMultiplier *= 2;
					calculateWeaponBonuses(player, game.m_weaponDatabase);
					auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return IncreasedRateOfFire == e.type; });
					player.bonuses[index] = player.bonuses.back();
					player.bonuses.pop_back();
				}
			},

			{
				BonusType::IncreasedMovementSpeed, "Runner", Time::secondsToMicros(5), 
				[](Player& player, RainbowlandGame& game)
				{
					auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return IncreasedMovementSpeed == e.type; });
					if(index == player.bonuses.size())
					{
						player.bonuses.emplace_back();
						player.bonuses[index].duration = 0;
						player.bonuses[index].timer.reset();
						player.bonuses[index].type = IncreasedMovementSpeed;
						player.maxSpeed *= 2;
					}
					player.bonuses[index].duration += game.m_bonusDatabase[IncreasedMovementSpeed].durationMicros;
				},
					[](Player& player, RainbowlandGame& game)
				{
					player.maxSpeed *= 0.5f;
					auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return IncreasedMovementSpeed == e.type; });
					player.bonuses[index] = player.bonuses.back();
					player.bonuses.pop_back();
				}
			},

			{
				BonusType::SlowTime, "Time slower", Time::secondsToMicros(8),
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
			}
		};

		std::sort(bonusDb.begin(), bonusDb.end(),
				  [](const Bonus& l, const Bonus& r)
		{
			return l.type < r.type;
		});
	}
}
