/*
 * CTokienPattern.h
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#ifndef CTOKIENPATTERN_H_
#define CTOKIENPATTERN_H_

#include <string>
#include "libmathing.h"


enum TypeOfMatching {m_full, m_regexp, m_Levenshtein};

class CTokienPattern
{
public:
	TypeOfMatching typeOfMatching;
	std::string value;
	CTokienPattern();
	virtual ~CTokienPattern();
};

#endif /* CTOKIENPATTERN_H_ */
