/*
 * CPunctuation.h
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#ifndef CPUNCTUATION_H_
#define CPUNCTUATION_H_

#include "CToken.h"

namespace patterns
{

enum PunctuationType {p_dot, p_comma, p_other, p_default };


class CPunctuation: public CToken
{
public:
	PunctuationType punctuationType;

	CPunctuation();
	CPunctuation(PunctuationType p);
	virtual ~CPunctuation();
};

}

#endif /* CPUNCTUATION_H_ */
