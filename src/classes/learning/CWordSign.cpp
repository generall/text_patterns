/*
 * CWordSign.cpp
 *
 *  Created on: 20 окт. 2013 г.
 *      Author: generall
 */

#include "CWordSign.h"

namespace patterns
{

CWordSign::CWordSign()
{
	//

}

double CWordSign::test(CText* text)
{
	if (word == NULL)
	{
		return 0;
	}
	else
	{
		auto i = text->statistics.find(word);
		if (i != text->statistics.end())
		{
			return i->second > 0 ? 1 : 0;
		}
		else
		{
			return 0;
		}
	}
}

void CWordSign::print()
{
	std::cout << word->value << std::endl;
}

CWordSign::~CWordSign()
{
}

CWordSign::CWordSign(CWord* _word)
{
	word = _word;
}

} /* namespace patterns */

