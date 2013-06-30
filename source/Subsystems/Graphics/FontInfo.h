#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Graphics
{
	class FontInfo
	{
	public:
		FontInfo();
		FontInfo(const char* name, uint32_t size, bool italic, uint32_t weight);
		FontInfo(const String& name, uint32_t size, bool italic, uint32_t weight);
		
		const String& getName() const;
		bool getItalic() const;
		uint32_t getWeight() const;
		uint32_t getSize() const;

		void setName(const char* name);
		void setName(const String& name);
		void setItalic(bool italicness);
		void setWeight(uint32_t weight);
		void setSize(uint32_t size);

	private:
		String _name;
		bool _italic;
		uint32_t _weight, _size;
	};
}