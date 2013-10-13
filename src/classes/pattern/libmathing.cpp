/*
 * libmathing.cpp
 *
 *  Created on: 13 окт. 2013 г.
 *      Author: generall
 */

#include "libmathing.h"

namespace patterns
{

std::wstring utf8to16(const std::string &s)
{
	wchar_t *wch = new wchar_t[s.size()];
	std::mbstowcs(wch, s.c_str(), s.size());
	std::wstring ret(wch);
	delete[] wch;
	return ret;
}

std::string utf16to8(const std::wstring &ws)
{
	char *ch = new char[ws.size() * sizeof(wchar_t)];
	wcstombs(ch, ws.c_str(), ws.size() * sizeof(wchar_t));
	std::string ret(ch);
	delete[] ch;
	return ret;
}

}
