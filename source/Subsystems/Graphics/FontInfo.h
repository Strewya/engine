#ifndef SUBSYSTEMS_GRAPHICS_FONTINFO_H_
#define SUBSYSTEMS_GRAPHICS_FONTINFO_H_
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
		FontInfo(const char* name, uint size, bool italic, uint weight);
		FontInfo(const String& name, uint size, bool italic, uint weight);
		
		const String& getName() const;
		bool getItalic() const;
		uint getWeight() const;
		uint getSize() const;

		void setName(const char* name);
		void setName(const String& name);
		void setItalic(bool italicness);
		void setWeight(uint weight);
		void setSize(uint size);

	private:
		String _name;
		bool _italic;
		uint _weight, _size;
	};
}

#endif //SUBSYSTEMS_GRAPHICS_FONTINFO_H_