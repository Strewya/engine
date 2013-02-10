#ifndef _SGActionmap_h_
#define _SGActionmap_h_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common header ***/
#include "Common.h"
#include "SGForwardDeclarations.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace SG
{
	//fwds
	//end fwds

	class Actionmap
	{
	private:
		typedef std::unordered_map<String, spAction> Container;
		Container _container;

		Actionmap(const Actionmap& am);
		Actionmap& operator=(const Actionmap& am);

	public:
		Actionmap();
		~Actionmap();
		
		bool Insert(spAction action, bool replace = true);
		Action* Get(const String& type);
		Action* operator[](const String& type);	//syntactic sugar for Get

		spAction Acquire(const String& name);

		bool Exists(const String& type) const;
		bool Remove(const String& type);
		void Clear();
	};
}

#endif //_SGActionmap_h_