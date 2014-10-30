#pragma once
/********************************************
*	class:	GameObjects
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Color.h>
#include <Util/Rect.h>
#include <Util/Transform.h>
/******* end header inclusion *******/

namespace Core
{
	class Time;

	struct Player
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		Vec2 velocity;
		Vec2 acceleration;
		uint32_t m_imageID;
	};

	typedef std::vector<Player> VPlayers;

	void movePlayers(const Time& timer, VPlayers& players, uint32_t activePlayerCount, const Rect& playingField);

	struct Monster
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		uint32_t m_imageID;
	};

	typedef std::vector<Monster> VMonsters;

	struct Bonus
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		uint32_t m_imageID;
	};

	typedef std::vector<Bonus> VBonuses;

	struct RayBullet
	{
		Vec2 origin;
		Vec2 position;
		Vec2 velocity;
		float travelled;
	};

	typedef std::vector<RayBullet> VRayBullets;

	void generateBullets(VRayBullets& bullets, uint32_t count, float spread, const Vec2& origin, const Vec2& target);
}
