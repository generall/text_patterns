/*
 * CWord.cpp
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#include "CWord.h"

namespace std
{

CWord::CWord() :limbType(l_default), wordType(w_default)
{
	// TODO Auto-generated constructor stub

}

CWord::CWord(LimbType l, WordType w):limbType(l), wordType(w)
{
}

CWord::~CWord()
{
	// TODO Auto-generated destructor stub
}

} /* namespace std */
