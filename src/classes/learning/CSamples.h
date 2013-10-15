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

namespace patterns
{

class CSamples
{



public:

	std::map<std::string, std::vector<CText *> > samples;

	void loadFromFiles(std::string dir, bool has_puncluation = false, bool calcStatistics = false);

	CSamples();
	virtual ~CSamples();
};

} /* namespace patterns */

#endif /* CSAMPLES_H_ */
