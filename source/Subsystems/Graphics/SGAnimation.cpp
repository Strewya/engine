	/*** personal header ***/
#include "SGAnimation.h"
	/*** extra headers ***/
	/*** end headers ***/

namespace SG
{
	Image* Animation::GetFrame(uint frame)
	{
		if(0 <= frame && frame < sequence.size())
		{
			return sequence[frame];
		}
		return NULL;
	}
}