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

class CTokenPattern
{
public:
	TypeOfMatching typeOfMatching;
	std::string value;
	uint delta;
	TokenType tokenType = type_word;

	bool compare(CToken token);

	CTokenPattern();
	CTokenPattern(uint d, std::string v);// auto Levenshtein constructor
	CTokenPattern(TypeOfMatching t);
	virtual ~CTokenPattern();
};

}

#endif /* CTOKIENPATTERN_H_ */
