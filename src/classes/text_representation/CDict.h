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
#include "CWord.h"


namespace patterns
{

class CDict
{

public:
	std::set<CWord*, CWordCompare> dictionary;
	std::vector<CWord*> dictionary_by_alpha;
	std::vector<CWord*> dictionary_by_length;
	bool reorganised = false;







	void addWord(CWord &word);
	void reorganiseToWork();

	void parseMysterm(const std::string &filename);
	int find_word(CWord &word);

	CDict();
	virtual ~CDict();
};

} /* namespace patterns */

#endif /* CDICT_H_ */
