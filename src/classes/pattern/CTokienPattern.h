/*
 * CTokienPattern.h
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#ifndef CTOKIENPATTERN_H_
#define CTOKIENPATTERN_H_

#include <iostream>
#include <string>
#include <regex>
#include "libmathing.h"
#include "../text_representation/CToken.h"

namespace patterns
{

enum TypeOfMatching {m_full, m_regexp, m_Levenshtein, m_type};

class CTokienPattern
{
public:
	TypeOfMatching typeOfMatching;
	std::string value;
	uint delta;
	TokenType tokenType = type_word;

	bool compre(CToken token);

	CTokienPattern();
	CTokienPattern(TypeOfMatching t);
	virtual ~CTokienPattern();
};

}

#endif /* CTOKIENPATTERN_H_ */
