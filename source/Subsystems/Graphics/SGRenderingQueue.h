#ifndef _SGRenderingQueue_h_
#define _SGRenderingQueue_h_
/********************************************
	class:	RenderingQueue
	usage:	
********************************************/
	/*** common header ***/
#include "Common.h"
#include "SGForwardDeclarations.h"
	/*** extra headers if needed ***/
#include <set>
	/*** end header inclusion ***/

namespace SG
{
    //fwds
	class Form;
	//end fwd
    
	class FormPtrCompare
	{
	public:
		bool operator()(const Form* a, const Form* b) const;
	};

	class RenderingQueue
	{
	public:
		void Add(Form* f);
		void Remove(Form* f);
		void Render();

	private:
		typedef std::set<Form*> FormSet;
		typedef std::map<int, FormSet> FormMap;

		FormMap _queue;
	};
}

#endif //_SGRenderingQueue_h_