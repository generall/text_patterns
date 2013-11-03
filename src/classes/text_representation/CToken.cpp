/*
 * CToken.cpp
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#include "CToken.h"

namespace patterns
{

CToken::CToken() :
		type(type_none)
{


}

CToken::CToken(TokenType t) :
		type(t)
{

}

CToken::~CToken()
{

}

}
