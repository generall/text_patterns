/*
 * CKnearest.h
 *
 *  Created on: 02 дек. 2013 г.
 *      Author: generall
 */

#ifndef CKNEAREST_H_
#define CKNEAREST_H_

#include "../classes/learning/CSamples.h"
#include "metric.hpp"

namespace patterns
{

class CKnearest
{

	int k = 15;
public:

	std::vector<std::pair<uint, std::vector<double> > > points;
	std::vector<std::string> clusters;

	CSamples *samples;
	CKnearest(CSamples *s);
	void create_points();
	std::string classify(std::vector<double> example, TClassifierInterface &metric);


	virtual ~CKnearest();
};

} /* namespace patterns */

#endif /* CKNEAREST_H_ */
