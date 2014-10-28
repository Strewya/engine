#pragma once
/********************************************
*	class:	GameObjects
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Color.h>
#include <Util/Rect.h>
#include <Util/Transform.h>
/******* end header inclusion *******/

namespace Core
{
	struct Player
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		uint32_t m_imageID;
	};

	struct Monster
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		uint32_t m_imageID;
	};

	struct Bonus
	{
		Transform transform;
		Color color;
		Rect boundingBox;
		uint32_t m_imageID;
	};

	struct RayBullet
	{
		Vec2 origin;
		Vec2 position;
		Vec2 velocity;
	};
}
