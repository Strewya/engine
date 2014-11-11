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
					if(timer.getCurMicros() >= Time::secondsToMicros(static_cast<float>(player.regenDelayForOneHealth)))
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
				[](Player& player, RainbowlandGame& game)
				{
					Circle radioactiveArea{player.transform.position, 5};
					for(auto& monsta : game.m_monsters)
					{
						Rect bbox = monsta.boundingBox;
						bbox.center = monsta.transform.position;
						if(isRectTouchingCircle(bbox, radioactiveArea))
						{
							monsta.health -= 1;
						}
					}
				}
			},
			{
				AmmoManiac, "Ammo Maniac",
				[](Player& player, RainbowlandGame& game)
				{
					player.ammoMultiplier = 1.2f;
					player.currentWeapon.ammo = static_cast<uint32_t>(player.ammoMultiplier*static_cast<float>(player.currentWeapon.ammo));
					player.currentWeapon.maxAmmo = player.currentWeapon.ammo;
				},
					[](Player& player, RainbowlandGame& game) {}
			},
			{
				Greaser, "Greaser",
				[](Player& player, RainbowlandGame& game) {},
				[](Player& player, RainbowlandGame& game) {}
			},
			{
				Fastloader, "Fastloader",
				[](Player& player, RainbowlandGame& game) {},
				[](Player& player, RainbowlandGame& game) {}
			},
			{
				MeanLeanExpMachine, "Lean Mean Exp Machine",
				[](Player& player, RainbowlandGame& game) {},
				[](Player& player, RainbowlandGame& game) {}
			},
			{
				HeavyRunner, "Heavy Runner",
				[](Player& player, RainbowlandGame& game) {},
				[](Player& player, RainbowlandGame& game) {}
			},
			{
				Dodger, "Dodger",
				[](Player& player, RainbowlandGame& game) {},
				[](Player& player, RainbowlandGame& game) {}
			},
			{
				HotTempered, "Hot Tempered",
				[](Player& player, RainbowlandGame& game) {},
				[](Player& player, RainbowlandGame& game) {}
			},
			{
				StationaryReloader, "Stationary Reloader",
				[](Player& player, RainbowlandGame& game) {},
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
