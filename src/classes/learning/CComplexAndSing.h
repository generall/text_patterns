/*
 * CComplexAndSing.h
 *
 *  Created on: 27 окт. 2013 г.
 *      Author: generall
 */

#ifndef CCOMPLEXANDSING_H_
#define CCOMPLEXANDSING_H_

#include "TSignature.h"
#include "CWordSign.h"
#include <vector>


namespace patterns
{

class CComplexAndSing: public patterns::TSignature
{
public:
	std::vector<CWord*> signs;

	virtual uint test(CText * text);
	CComplexAndSing();
	CComplexAndSing(const std::vector<CWord*> &signs_ );
	virtual ~CComplexAndSing();
};

} /* namespace patterns */

#endif /* CCOMPLEXANDSING_H_ */
