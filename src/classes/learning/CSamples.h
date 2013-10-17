/*
 * CSamples.h
 *
 *  Created on: 15 окт. 2013 г.
 *      Author: generall
 */

#ifndef CSAMPLES_H_
#define CSAMPLES_H_

#include <vector>
#include <utility>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "../text_representation/CText.h"
#include "../pattern/CTextPattern.h"
#include "../pattern/CPatternComplex.h"


namespace patterns
{

class CSamples
{



public:

	std::map<std::string, std::vector<CText *> > samples;

	std::map<std::string, std::vector<std::pair<CWord *, int> > >  statistic;

	void loadFromFiles(std::string dir, bool has_puncluation = false, bool calcStatistics = false);

	void testPattern(CTextPattern &pattern);
	void testComplex(CPatternComplex &pattern);

	void calcGroupStat();

	void summStatistics(std::map<CWord*, int, CWordCompare> &s1, const std::map<CWord*, int, CWordCompare> &s2);

	CSamples();
	virtual ~CSamples();
};

} /* namespace patterns */

#endif /* CSAMPLES_H_ */
