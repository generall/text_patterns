/*
 * CComplexAndSing.cpp
 *
 *  Created on: 27 окт. 2013 г.
 *      Author: generall
 */

#include "CComplexAndSing.h"

namespace patterns
{

CComplexAndSing::CComplexAndSing()
{
	// TODO Auto-generated constructor stub

}

uint CComplexAndSing::test(CText* text)
{
	for (auto x : signs)
	{
		if (CWordSign(x).test(text) == 0)
		{
			return 0;
		}
	}
	return 1;
}

CComplexAndSing::CComplexAndSing(const std::vector<CWord*> &signs_)
{
	signs = signs_;
}

void CComplexAndSing::print()
{
	std::cout << "------&&------" << std::endl;
	for (auto x : signs)
	{
		std::cout << x->value << std::endl;
	}
}

CComplexAndSing::~CComplexAndSing()
{
	// TODO Auto-generated destructor stub
}

} /* namespace patterns */
