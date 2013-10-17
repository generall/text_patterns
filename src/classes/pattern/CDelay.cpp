/*
 * CDelay.cpp
 *
 *  Created on: 07 окт. 2013 г.
 *      Author: generall
 */

#include "CDelay.h"

namespace patterns
{

CDelay::CDelay()
{
	maxDelayNumber = 0;
	// TODO Auto-generated constructor stub

}

bool CDelay::isDelayAccaptable(const CToken &t) const
{
	for(CTokenPattern p : unacceptablePatterns)
	{
		if(p.compare(t))
		{
			return false; // unacceptable token detected
		}
	}
	return true; // all clear
}

void CDelay::addUnacceptablePattern(CTokenPattern& p)
{
	unacceptablePatterns.push_back(p);
}


CDelay::~CDelay()
{
	// TODO Auto-generated destructor stub
}

} /* namespace patterns */

patterns::CDelay::CDelay(uint n)
{
	maxDelayNumber = n;
}
