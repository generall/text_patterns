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
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

namespace patterns
{

class CText
{

	CDict stop_list;
	std::string stoplist;

public:

	std::string file_name;
	std::vector<CToken*> text;
	std::map<CWord*, int, CWordCompare> statistics;
	std::vector<std::pair<CWord *, int>> stat_by_friquency;


	void loadFromMytsem(const std::string &dir, const std::string &filename, bool has_punctuation = true);
	void loadFromXml(const std::string &dir, const std::string &filename);
	void loadFromXml(const std::string &filename);


	void setStoplist(const std::string &dir,const std::string &st_list);
	void initStopDic(CDict &dict);

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
