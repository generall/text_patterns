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


namespace patterns
{

class CPatternComplex
{
public:
	std::vector<std::vector<CTextPattern> > DNF;

	bool cmp(std::vector<CToken *> &text);

	CPatternComplex();
	virtual ~CPatternComplex();
};

} /* namespace patterns */

#endif /* CPATTERNCOMPLEX_H_ */
