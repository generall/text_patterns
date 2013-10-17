/*
 * TPatternInterface.h
 *
 *  Created on: 17 окт. 2013 г.
 *      Author: generall
 */

#ifndef TPATTERNINTERFACE_H_
#define TPATTERNINTERFACE_H_

#include <vector>
#include "../text_representation/CToken.h"

namespace patterns
{

class TPatternInterface
{
public:
	virtual uint cmp(std::vector<CToken *> &text) = 0;
	virtual uint cmp(std::vector<CToken> &text) = 0;

	virtual ~TPatternInterface();
};

} /* namespace patterns */

#endif /* TPATTERNINTERFACE_H_ */
