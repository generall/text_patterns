/*
 * CToken.h
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#ifndef CTOKEN_H_
#define CTOKEN_H_

#include <string>

namespace patterns
{

enum TokenType
{
	type_word, type_punctuation, type_number, type_none
};



class CToken
{
	TokenType type;

public:

	std::wstring value;

	CToken();
	CToken(TokenType t);

	virtual ~CToken();

	TokenType getType() const
	{
		return type;
	}
	void setType(TokenType type)
	{
		this->type = type;
	}

};

}
#endif /* CTOKEN_H_ */
