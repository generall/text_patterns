/*
 * CDelay.h
 *
 *  Created on: 07 окт. 2013 г.
 *      Author: generall
 */

#ifndef CDELAY_H_
#define CDELAY_H_

#include <vector>
#include "../text_representation/CToken.h"
#include "CTokienPattern.h"

namespace patterns
{

class CDelay
{
public:
	uint maxDelayNumber;
	std::vector<CTokenPattern> unacceptablePatterns;


	bool isDelayAccaptable(CToken &t); //true, if delay acceptable
	void addUnacceptablePattern(CTokenPattern &p);

	CDelay();
	virtual ~CDelay();
};

} /* namespace patterns */

#endif /* CDELAY_H_ */
