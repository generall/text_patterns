/*
 * CDict.h
 *
 *  Created on: 10 окт. 2013 г.
 *      Author: generall
 */

#ifndef CDICT_H_
#define CDICT_H_

#include <vector>
#include <fstream>
#include <regex>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <iconv.h>
#include <wchar.h>
#include <wctype.h>
#include "CWord.h"
#include "../pattern/libmathing.h"


namespace patterns
{

class CDict
{

	std::set<std::string> dictionary;


public:


	void addWord(const std::string &word);
	void loadSimple(const std::string &filename);
	int findWord(const std::string &word);


	CDict();
	virtual ~CDict();
};

} /* namespace patterns */

#endif /* CDICT_H_ */
