/*
 * CTokienPattern.cpp
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#include "CTokienPattern.h"

namespace patterns
{

CTokenPattern::CTokenPattern() :
		typeOfMatching(m_Levenshtein)
{
	delta = 0;
	// TODO Auto-generated constructor stub

}

bool CTokenPattern::compare(CToken token)
{
	switch (typeOfMatching)
	{
	case m_Levenshtein:
		return levenshtein_distance(value, token.value) < delta;
	case m_full:
		return value == token.value && tokenType == token.getType();
	case m_regexp:
	{
		std::regex txt_regex(value);
		return std::regex_match(token.value, txt_regex);
	}
	case m_type:
		return tokenType == token.getType();
	}
	return false;
}

CTokenPattern::CTokenPattern(uint d, std::string v)
{
	delta = d;
	value = v;
	typeOfMatching = m_Levenshtein;
}

CTokenPattern::~CTokenPattern()
{
// TODO Auto-generated destructor stub
}

CTokenPattern::CTokenPattern(TypeOfMatching t) :
		typeOfMatching(t)
{
	delta = 0;
}

}