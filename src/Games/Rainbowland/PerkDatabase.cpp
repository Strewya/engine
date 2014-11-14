//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/PerkDatabase.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Rainbowland.h>
#include <Util/Circle.h>
#include <Util/CollisionChecks.h>
#include <Util/Time.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	void initPerkDatabase(VPerks& perkDb)
	{
		Time timer;
		timer.reset();
		perkDb =
		{
			{
				Regeneration, "Regeneration",
				[=](Player& player, RainbowlandGame& game)
				{
					player.regenDelayForOneHealth = 4;
				},
					[=](Player& player, RainbowlandGame& game) mutable
				{
					timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
					if(timer.getCurrentMicros() >= Time::secondsToMicros(static_cast<float>(player.regenDelayForOneHealth)))
					{
						timer.reset();
						player.health += 1;
						clamp<int32_t>(0, player.maxHealth, player.health);
					}
				}
			},
			{
				PoisonBullets, "Poison Bullets",
				[](Player& player, RainbowlandGame& game)
				{
					player.bonusDamage += 5;
					player.currentWeapon.damage += player.bonusDamage;
				},
					[](Player& player, RainbowlandGame& game){}
			},
			{
				Radioactive, "Radioactive",
				[](Player& player, RainbowlandGame& game) {},
				[=](Player& player, RainbowlandGame& game) mutable
				{
					timer.updateBy(game.m_gameplayTimer.getDeltaMicros());
					if(timer.getCurrentMicros() > Time::secondsToMicros(0.5f))
					{
						timer.reset();
						Circle radioactiveArea{player.transform.position, 5};
						for(auto& monsta : game.m_monsters)
						{
							auto bCollider = monsta.collisionData;
							bCollider.center = monsta.transform.position;
							if(isCircleTouchingCircle(bCollider, radioactiveArea))
							{
								monsta.health -= 1;
							}
						}
					}
				}
			},
			{
				AmmoManiac, "Ammo Maniac",
				[](Player& player, RainbowlandGame& game)
				{
					player.ammoMultiplier = 1.2f;
					player.currentWeapon.maxAmmo = static_cast<uint32_t>(player.ammoMultiplier*static_cast<float>(player.currentWeapon.maxAmmo));
				},
					[](Player& player, RainbowlandGame& game) {}
			},
			{
				InstantWinner, "Instant winner",
				[](Player& player, RainbowlandGame& game)
				{
					grantExperience(5000, game.m_players);
				},
				[](Player& player, RainbowlandGame& game) {}
			},
			{
				Fastloader, "Fastloader",
				[](Player& player, RainbowlandGame& game)
				{
					player.reloadMultiplier *= 0.7f;
				},
					[](Player& player, RainbowlandGame& game) {}
			},
			{
				ReflexBoosted, "Reflex boosted",
				[](Player& player, RainbowlandGame& game)
				{
					game.m_restoreTimeScaleAfterPerkMode *= 0.9;
				},
					[](Player& player, RainbowlandGame& game) {}
			}
		};

		std::sort(perkDb.begin(), perkDb.end(),
				  [](const Perk& l, const Perk& r)
		{
			return l.type < r.type;
		});
	}
}
