/*
 * CTokienPattern.h
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#ifndef CTOKIENPATTERN_H_
#define CTOKIENPATTERN_H_

namespace std
{

enum TypeOfMatching {m_full, m_regexp, m_Levenshtein};

class CTokienPattern
{
public:
	TypeOfMatching typeOfMatching;
	string value;
	CTokienPattern();
	virtual ~CTokienPattern();
};

} /* namespace std */
#endif /* CTOKIENPATTERN_H_ */
