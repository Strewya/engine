#pragma once
/********************************************
*	class:	AnimationCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <DataStructs/Animation.h>
/******* end header inclusion *******/

namespace Core
{
	class DataFile;
	class SpritesheetCache;

	class AnimationCache
	{
	public:
		bool init(SpritesheetCache& spritesheets);
		bool shutdown();

		bool loadAnimation(DataFile& df);

	private:
		SpritesheetCache* m_spritesheets;

		std::vector<Animation> m_animations;
	};
}
