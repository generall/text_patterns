/*
 * CWord.h
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#ifndef CWORD_H_
#define CWORD_H_

#include "CToken.h"

namespace patterns
{

enum WordType
{
	w_verb, w_noun, w_adjective, w_default
};
enum LimbType
{
	l_subject, l_predicate, l_addition, l_circumstance, l_default
};

class CWord: public CToken
{
public:

	LimbType limbType;
	WordType wordType;

	CWord();
	CWord(std::string s);
	CWord(LimbType l, WordType w);
	virtual ~CWord();
};

class CWordCompare
{
public:
	bool operator()(CWord *w1, CWord *w2)
	{
		return w1->value.compare(w2->value) > 0;
	}
};

class CWordCompareLength
{
public:
	bool operator()(CWord *w1, CWord *w2)
	{
		return (w1->value.length() < w2->value.length());
	}
};


}
#endif /* CWORD_H_ */
