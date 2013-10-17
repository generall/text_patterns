/*
 * CPatternComplex.h
 *
 *  Created on: 16 окт. 2013 г.
 *      Author: generall
 */

#ifndef CPATTERNCOMPLEX_H_
#define CPATTERNCOMPLEX_H_

#include <vector>
#include <iostream>
#include "CTextPattern.h"
#include "TPatternInterface.h"

namespace patterns
{

class CPatternComplex: public TPatternInterface
{
public:
	std::vector<std::vector<CTextPattern> > DNF;

	virtual uint cmp(std::vector<CToken *> &text) const;
	virtual uint cmp(std::vector<CToken> &text) const;

	CPatternComplex();
	virtual ~CPatternComplex();
};

} /* namespace patterns */

#endif /* CPATTERNCOMPLEX_H_ */
