#pragma once
/********************************************
	class:	Converter
	usage:	
********************************************/
	/*** common header ***/
#include <Engine/Defines.h>
#include <Engine/FwdDecl.h>
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Input
{
	class Converter
	{
	public:
		Converter(double minIn, double maxIn, double minOut, double maxOut);
		
		template<typename RangeType> double convert(RangeType value) const
		{
			double v = static_cast<double>(value);
			if(v < m_minInput)
				v = m_minInput;
			else if(v > m_maxInput)
				v = m_maxInput;

			double interpolationfactor = (v - m_minInput) / (m_maxInput - m_minInput);
			return static_cast<RangeType>((interpolationfactor * (m_maxOutput - m_minOutput)) + m_minOutput);
		}

		double m_minInput;
		double m_maxInput;

		double m_minOutput;
		double m_maxOutput;
	};
}
