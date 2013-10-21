/*
 * CText.h
 *
 *  Created on: 13 окт. 2013 г.
 *      Author: generall
 */

#ifndef CTEXT_H_
#define CTEXT_H_

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <cstring>
#include <utility>
#include "CPunctuation.h"
#include "CWord.h"
#include "../pattern/libmathing.h"
#include "../pattern/TPatternInterface.h"
#include "CDict.h"

namespace patterns
{

class CText
{

	std::vector<CToken*> text;

public:

	std::map<CWord*, int, CWordCompare> statistics;
	std::vector<std::pair<CWord *, int>> stat_by_friquency;
	std::string stoplist;

	void loadFromMytsem(const std::string &dir, const std::string &filename, bool has_punctuation = true);
	void performStatistics();
	int testPatetrn(const TPatternInterface &pattern);


	CText();
	virtual ~CText();
};

class StatByFriquencyCmp
{
public:
	bool operator()(std::pair<CWord *, int> w1, std::pair<CWord *, int> w2)
	{
		return (w1.second > w2.second);
	}
};

} /* namespace patterns */

#endif /* CTEXT_H_ */
