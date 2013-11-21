/*
 * TestClassifier.hpp
 *
 *  Created on: 30 окт. 2013 г.
 *      Author: generall
 */

#ifndef TESTCLASSIFIER_HPP_
#define TESTCLASSIFIER_HPP_

#include "../classes/learning/CSamples.h"
#include "metric.hpp"

namespace patterns
{

double testClassifier(CSamples &teaching_selection, CSamples &examples,
		TClassifierInterface *classifier);

void executeClassifier(CSamples &teaching_selection, CSamples &examples,
		TClassifierInterface *classifier, const std::string file_name);

}
#endif /* TESTCLASSIFIER_HPP_ */
