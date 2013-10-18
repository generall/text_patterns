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
#include "../pattern/TPatternInterface.h"

namespace patterns
{

class CSamples
{

public:

	std::map<std::string, std::vector<CText *> > samples;

	std::map<std::string, std::vector<std::pair<CWord *, int> > > statistic;

	std::map<std::string, std::vector<std::pair<CWord *, int> > > lastStatistic;
	std::map<std::string, std::map<CWord *, int, CWordCompare> >  lastStatisticByWord;


	std::map<std::string, std::vector<bool> > lastAcceptedMask;
	std::map<std::string, std::pair<uint, uint> > lastPatterStatistic;



	void loadFromFiles(std::string dir, bool has_puncluation = false, bool calcStatistics = false);

	void testPattern(const TPatternInterface &pattern);
	void testPattern(const TPatternInterface &pattern,
			std::map<std::string, std::vector<bool> > &mask, bool accept = false,
			bool renew = false);

	void calcGroupStat();
	void calcGroupStat(std::map<std::string, std::vector<bool> > &mask, bool accept = true);

	void summStatistics(std::map<CWord*, int, CWordCompare> &s1,
			const std::map<CWord*, int, CWordCompare> &s2);

	CSamples();
	virtual ~CSamples();
};

} /* namespace patterns */

#endif /* CSAMPLES_H_ */
