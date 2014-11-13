//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/BonusDatabase.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Rainbowland.h>
#include <Util/Clock.h>
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
					player.bonuses.pop_back();
				},
					[](Player& player, RainbowlandGame& game) {},
					[](Player& player, RainbowlandGame& game) {}
			},
			{
				BonusType::IncreasedMovementSpeed, "Speedy", Clock::secondsToMicros(5),
				[](Player& player, RainbowlandGame& game)
				{
					auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return IncreasedMovementSpeed == e.type; });
					if(index == player.bonuses.size())
					{
						player.rateOfFireMultiplier *= 0.5f;
						player.reloadMultiplier *= 0.5f;
						player.currentWeapon.fireDelay = static_cast<uint64_t>(0.5f*static_cast<float>(player.currentWeapon.fireDelay));
						player.currentWeapon.reloadDelay = static_cast<uint64_t>(0.5f*static_cast<float>(player.currentWeapon.reloadDelay));
					}
					player.bonuses[index].timer.addExtraMicros(game.m_bonusDatabase[IncreasedMovementSpeed].duration);
				},
					[](Player& player, RainbowlandGame& game) {},
					[](Player& player, RainbowlandGame& game)
				{
					player.rateOfFireMultiplier *= 2;
					player.reloadMultiplier *= 2;
					player.currentWeapon.fireDelay = static_cast<uint64_t>(2 * static_cast<float>(player.currentWeapon.fireDelay));
					player.currentWeapon.reloadDelay = static_cast<uint64_t>(2 * static_cast<float>(player.currentWeapon.reloadDelay));
				}
			},
			{
				BonusType::IncreasedRateOfFire, "Shooter", Clock::secondsToMicros(5),
				[](Player& player, RainbowlandGame& game)
				{
					auto index = filterFind(player.bonuses, [=](const ActiveBonus& e){return IncreasedRateOfFire == e.type; });
					if(index == player.bonuses.size())
					{
						player.maxVelocity *= 2;
					}
					player.bonuses[index].timer.addExtraMicros(game.m_bonusDatabase[IncreasedRateOfFire].duration);
				},
					[](Player& player, RainbowlandGame& game) {},
					[](Player& player, RainbowlandGame& game)
				{
					player.maxVelocity *= 0.5f;
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
