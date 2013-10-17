/*
 * CComplexGenerator.h
 *
 *  Created on: 17 окт. 2013 г.
 *      Author: generall
 */

#ifndef CCOMPLEXGENERATOR_H_
#define CCOMPLEXGENERATOR_H_

#include <vector>
#include <iostream>
#include <string>
#include "CSamples.h"
#include "../pattern/CPatternComplex.h"

namespace patterns
{

class CComplexGenerator
{

public:

	CPatternComplex generatePattern(const std::string &classter, CSamples &samples, uint maxlen,
			CPatternComplex currentComplex = NULL);
	CComplexGenerator();
	virtual ~CComplexGenerator();
};

} /* namespace patterns */

#endif /* CCOMPLEXGENERATOR_H_ */
