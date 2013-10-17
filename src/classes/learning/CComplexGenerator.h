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
#include <cmath>
#include "CSamples.h"
#include "../pattern/CPatternComplex.h"

namespace patterns
{

class CComplexGenerator
{
	bool nextCombination(std::vector<int> & a, int n);
	uint max_word_to_consider = 50;

	double entropy(std::vector<double> &data);

public:

	CPatternComplex generatePattern(const std::string &classter, CSamples &samples, uint maxlen);
	CComplexGenerator();
	virtual ~CComplexGenerator();
};

} /* namespace patterns */

#endif /* CCOMPLEXGENERATOR_H_ */
