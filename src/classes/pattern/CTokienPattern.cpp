/*
 * CTokienPattern.cpp
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#include "CTokienPattern.h"

namespace patterns
{

CTokienPattern::CTokienPattern() :
		typeOfMatching(m_Levenshtein)
{
	delta = 0;
	// TODO Auto-generated constructor stub

}

bool CTokienPattern::compre(CToken token)
{
	switch (typeOfMatching)
	{
	case m_Levenshtein:
		return levenshtein_distance(value, token.value) < delta;
	case m_full:
		return value == token.value;
	case m_regexp:
		std::regex txt_regex(value);
		return std::regex_match( token.value, txt_regex);
	}
	return false;
}

CTokienPattern::~CTokienPattern()
{
// TODO Auto-generated destructor stub
}

CTokienPattern::CTokienPattern(TypeOfMatching t) :
		typeOfMatching(t)
{
	delta = 0;
}

}
