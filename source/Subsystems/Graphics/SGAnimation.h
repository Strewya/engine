#ifndef _SGAnimation_h_
#define _SGAnimation_h_
/********************************************
	class:	Animation
	usage:	Primary concept of an animation, composed of a sequence of images,
			must contain at least one image
********************************************/
	/*** common header ***/
#include "Common.h"
#include "SGForwardDeclarations.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace SG
{
	//forward declarations
	class Image;
	//end fwd

	class Animation
	{
	public:
		virtual ~Animation() {};

		std::vector<Image*> sequence;
		String name;
		RepeatMode mode;

		Image* GetFrame(uint frame);
	};
}

#endif //_SGAnimation_h_