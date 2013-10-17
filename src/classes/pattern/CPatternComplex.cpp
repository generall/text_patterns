/*
 * CPatternComplex.cpp
 *
 *  Created on: 16 окт. 2013 г.
 *      Author: generall
 */

#include "CPatternComplex.h"

namespace patterns
{

CPatternComplex::CPatternComplex()
{
	// TODO Auto-generated constructor stub

}

bool CPatternComplex::cmp(std::vector<CToken*>& text)
{
	for(auto x: DNF)
	{
		bool flag = true;
		for(auto y: x)
		{
			flag &= y.compare(text) > 0 ? true : false;
		}
		if(flag)
			return flag;
	}
	return false;
}

CPatternComplex::~CPatternComplex()
{
	// TODO Auto-generated destructor stub
}

} /* namespace patterns */
