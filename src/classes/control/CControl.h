/*
 * CControl.h
 *
 *  Created on: 21 нояб. 2013 г.
 *      Author: generall
 */

#ifndef CCONTROL_H_
#define CCONTROL_H_

#include "../pattern/CTextPattern.h"
#include "../pattern/CPatternComplex.h"
#include "../text_representation/CWord.h"
#include "../text_representation/CDict.h"
#include "../text_representation/CText.h"
#include "../learning/CSamples.h"
#include "../../functions/metric.hpp"
#include "../../functions/TestClassifier.hpp"

#include "CConfig.h"
#include <string>
#include <map>

namespace patterns
{

class CControl
{
	int argc;
	char ** argv;
	CConfig config;
	bool do_classify;
	std::string conf_filename;
	std::string output_filename;



public:

	void init(int _argc, char **_argv);
	void classify();
	void keywords();
	int execute();
	CControl();
	virtual ~CControl();
};

} /* namespace patterns */

#endif /* CCONTROL_H_ */
