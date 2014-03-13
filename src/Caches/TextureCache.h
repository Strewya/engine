#pragma once
/********************************************
*	class:	TextureCache
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
	class GraphicsSystem;

	class TextureCache
	{
	public:
		bool init(GraphicsSystem& graphics);
		bool shutdown();

		uint32_t getTextureID(const char* path);
		Vec2 getTextureDimensions(uint32_t texID);
		
	private:
		GraphicsSystem* m_graphics;

		std::vector<std::pair<std::string, uint32_t>> m_loadedTextures;
	};
}
