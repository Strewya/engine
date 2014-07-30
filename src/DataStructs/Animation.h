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
/******* end header inclusion *******/

namespace Core
{
	class DataFile;
	class ImageCache;

	class Animation
	{
	public:
		uint64_t m_duration;
		std::vector<uint32_t> m_sequence;
		std::string m_name;
		bool m_loops;
	};

	class AnimationDefaults
	{
	public:
		float duration;
		bool loops;
	};

	bool parseAnimation(Animation& outAnimation, DataFile& file, const AnimationDefaults& defaults, const ImageCache& images);
}
