/*
 * CDict.cpp
 *
 *  Created on: 10 окт. 2013 г.
 *      Author: generall
 */

#include "CDict.h"

namespace patterns
{

} /* namespace patterns */

void patterns::CDict::addWord(const std::string& word)
{
	dictionary.insert(word);
}

void patterns::CDict::loadSimple(const std::string& filename)
{
	std::setlocale(LC_ALL, "en_US.UTF-8");
	std::fstream in(filename.c_str(), std::ios::in);
	char line[1024];

	while (!in.eof())
	{
		in.getline(line, 1024);
		std::string first_form(line);
		addWord(first_form);
	}
}

int patterns::CDict::findWord(const std::string& word)
{
	if (dictionary.find(word) != dictionary.end())
	{
		return 0;
	}
	return -1;
}

patterns::CDict::CDict()
{
}

patterns::CDict::~CDict()
{
}
