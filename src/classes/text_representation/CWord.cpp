/*
 * CWord.cpp
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#include "CWord.h"
namespace patterns
{

CWord::CWord() :
		limbType(l_default), wordType(w_default)
{
	setType(type_word);
}

CWord::CWord(LimbType l, WordType w) :
		limbType(l), wordType(w)
{
	setType(type_word);
}

CWord::CWord(std::string s) :
		limbType(l_default), wordType(w_default)
{
	value = s;
	setType(type_word);
}

CWord::~CWord()
{
	// TODO Auto-generated destructor stub
}

}
