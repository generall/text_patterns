/*
 * CWord.h
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#ifndef CWORD_H_
#define CWORD_H_

#include "CToken.h"

enum WordType {w_verb, w_noun, w_adjective, w_default };
enum LimbType {l_subject, l_predicate, l_addition, l_circumstance, l_default};

namespace std
{

class CWord: public std::CToken
{
public:

	LimbType limbType;
	WordType wordType;

	CWord();
	CWord(LimbType l, WordType w);
	virtual ~CWord();
};

} /* namespace std */
#endif /* CWORD_H_ */
