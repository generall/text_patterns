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

	std::set<CWord*, CWordCompare> dictionary;
	std::vector<CWord*> dictionary_by_alpha;
	std::vector<CWord*> dictionary_by_length;
	bool reorganised = false;

	void reorganiseToWork();
	int getMaxLevenshteinDist(std::wstring val);

public:


	void addWord(CWord &word);

	CWord nearestLevenshteinWord(CWord &word);
	void parseMysterm(const std::string &filename);
	int findWord(CWord &word);
	void analysis();


	CDict();
	virtual ~CDict();
};

} /* namespace patterns */

#endif /* CDICT_H_ */
