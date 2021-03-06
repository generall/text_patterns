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
#include "CTokenPattern.h"
#include "TPatternInterface.h"

namespace patterns
{

class CTextPattern: public virtual TPatternInterface
{
public:
	std::list<std::pair<CDelay, CTokenPattern> > pattern;

	void addBack(std::pair<CDelay, CTokenPattern> &p);
	void add(const CTokenPattern &p);// for single pattern
	void del();// delete right
	void addFront(std::pair<CDelay, CTokenPattern> &p);
	uint compare(std::vector<CToken> &data) const;
	uint compare(std::vector<CToken*> &data) const;
	virtual uint cmp(std::vector<CToken> &data) const;
	virtual uint cmp(std::vector<CToken*> &data) const;

	// TODO написать тест на все это дело.
	CTextPattern();
	virtual ~CTextPattern();
};

} /* namespace patterns */

#endif /* CTEXTPATTERN_H_ */
