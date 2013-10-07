/*
 * CTextPattern.h
 *
 *  Created on: 07 окт. 2013 г.
 *      Author: generall
 */

#ifndef CTEXTPATTERN_H_
#define CTEXTPATTERN_H_

#include <list>
#include <vector>
#include <utility> //for std::pair
#include "CDelay.h"
#include "CTokienPattern.h"

namespace patterns
{

class CTextPattern
{
public:
	std::list<std::pair<CDelay, CTokienPattern> > pattern;

	void addBack(std::pair<CDelay, CTokienPattern> &p);
	void addFront(std::pair<CDelay, CTokienPattern> &p);
	uint compare(std::vector<CToken> &data);

	// TODO написать тест на все это дело.
	CTextPattern();
	virtual ~CTextPattern();
};

} /* namespace patterns */

#endif /* CTEXTPATTERN_H_ */
