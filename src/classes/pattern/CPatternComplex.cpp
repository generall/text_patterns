/*
 * CPatternComplex.cpp
 *
 *  Created on: 16 окт. 2013 г.
 *      Author: generall
 */

#include "CPatternComplex.h"

namespace patterns
{

CPatternComplex::CPatternComplex()
{
	// TODO Auto-generated constructor stub

}

uint CPatternComplex::cmp(std::vector<CToken*>& text) const
{
	if (DNF.size() == 0)
		return 1; //accep by default

	for (auto x : DNF)
	{
		bool flag = true;
		for (auto y : x)
		{
			flag &= y.compare(text) > 0 ? true : false;
		}
		if (flag)
			return 1;
	}
	return 0;
}

uint CPatternComplex::cmp(std::vector<CToken>& text) const
{
	if (DNF.size() == 0)
		return 1;

	for (auto x : DNF)
	{
		bool flag = true;
		for (auto y : x)
		{
			flag &= y.compare(text) > 0 ? true : false;
		}
		if (flag)
			return 1;
	}
	return 0;
}

CPatternComplex::~CPatternComplex()
{
	// TODO Auto-generated destructor stub
}

} /* namespace patterns */

void patterns::CPatternComplex::add(const CTextPattern &cTextPattern)
{
	std::vector<patterns::CTextPattern> impl;
	impl.push_back(cTextPattern);
	DNF.push_back(impl);
}

void patterns::CPatternComplex::del()
{
	DNF.erase(DNF.end() - 1);
}
