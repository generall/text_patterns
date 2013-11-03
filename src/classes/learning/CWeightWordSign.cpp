/*
 * CWeightWordSign.cpp
 *
 *  Created on: 03 нояб. 2013 г.
 *      Author: generall
 */

#include "CWeightWordSign.h"

namespace patterns
{

CWeightWordSign::CWeightWordSign()
{
	// TODO Auto-generated constructor stub

}

uint CWeightWordSign::test(CText* text)
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
			return i->second;
		}
		else
		{
			return 0;
		}
	}

}

CWeightWordSign::CWeightWordSign(CWord* word) :
		CWordSign(word)
{
}

CWeightWordSign::~CWeightWordSign()
{
	// TODO Auto-generated destructor stub
}

} /* namespace patterns */
