//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Modules/Input/Service/Converter.h>
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Input
{
	Converter::Converter(double minIn, double maxIn, double minOut, double maxOut)
		: m_minInput(minIn), m_maxInput(maxIn), m_minOutput(minOut), m_maxOutput(maxOut)
	{
		if(m_maxInput < m_minInput)
			std::swap(m_maxInput, m_minInput);
		if(m_maxOutput < m_minOutput)
			std::swap(m_maxOutput, m_minOutput);
	}
}
