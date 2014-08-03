#pragma once
/********************************************
*	class:	Animation
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Loaders/Defines.h>
/******* end header inclusion *******/

namespace Core
{
	class ImageCache;
	class LuaStack;

	class Animation
	{
	public:
		uint64_t m_duration;
		std::vector<uint32_t> m_sequence;
		std::string m_name;
		size_t m_fileHash;
		bool m_loops;
	};

	class AnimationDefaults
	{
	public:
		float duration;
		bool loops;
	};

	LoadResult loadAnimation(Animation& outAnimation, LuaStack& lua, size_t fileHash, const AnimationDefaults& defaults, ImageCache& images);
	void unloadAnimation(Animation& outAnimation);
}
