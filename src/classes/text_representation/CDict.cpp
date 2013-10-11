/*
 * CDict.cpp
 *
 *  Created on: 10 окт. 2013 г.
 *      Author: generall
 */

#include "CDict.h"

namespace patterns
{

CDict::CDict()
{
	// TODO Auto-generated constructor stub

}

void CDict::addWord(CWord& word)
{
	CWord *t = new CWord(word);
	dictionary.insert(t);
	reorganised = false;
}

int CDict::findWord(CWord& word)
{
	if (!reorganised)
		reorganiseToWork();

	std::vector<CWord*>::iterator i = std::lower_bound(
			dictionary_by_alpha.begin(), dictionary_by_alpha.end(), &word,
			CWordCompare());
	if (i == dictionary_by_alpha.end())
	{
		return -1;
	}
	if (i == dictionary_by_alpha.begin())
	{
		if (!std::binary_search(dictionary_by_alpha.begin(),
				dictionary_by_alpha.end(), &word, CWordCompare()))
		{
			return -1;
		}
	}
	return i - dictionary_by_alpha.begin();
}

void CDict::reorganiseToWork()
{
	for (CWord *w : dictionary)
	{
		dictionary_by_alpha.push_back(w);
		dictionary_by_length.push_back(w);
	}
	std::sort(dictionary_by_alpha.begin(), dictionary_by_alpha.end(),
			CWordCompare());

	std::cout << "SEGFAULT not here" << std::endl;
	std::sort(dictionary_by_length.begin(), dictionary_by_length.end(),
			CWordCompareLength());
	for (auto w : dictionary_by_length)
	{
		std::cout << w->value << std::endl;
	}

	std::cout << "SEGFAULT not here" << std::endl;
	reorganised = true;
}

void CDict::parseMysterm(const std::string& filename)
{
	std::setlocale(LC_ALL, "en_US.UTF-8");
	std::fstream in(filename.c_str(), std::ios::in);
	char line[1024];

	while (!in.eof())
	{
		in.getline(line, 1024);
		std::string s(line), first_form, get_form, type;
		std::smatch m;
		std::stringstream text_strteam(line);

		std::getline(text_strteam, get_form, ' ');
		std::getline(text_strteam, first_form, ' ');
		std::getline(text_strteam, type);

		//std::cout << "1: " << get_form << " 2: " << first_form << " 3: " << type
		//		<< std::endl;
		CWord w(first_form);
		w.wordType = w_default;
		if ("S" == type)
			w.wordType = w_noun;
		if ("A" == type)
			w.wordType = w_adjective;
		if ("V" == type)
			w.wordType = w_verb;

		addWord(w);
	}
}

CDict::~CDict()
{
	for (CWord *w : dictionary)
	{
		delete w;
	}
}



int CDict::getMaxLevenshteinDist(std::string val)
{
	// TODO add calculation with dictionary
	return 2;
}

CWord CDict::nearestLevenshteinWord(CWord& word)
{
	//Получить диапазон из массива, для которого минимальное рассмояние Левенштейна не больше чем MaxLevenshteinDist

	int lower_length_bound = std::max( word.value.length() - getMaxLevenshteinDist(word.value), 0);
	int upper_length_bound = word.value.length() + getMaxLevenshteinDist(word.value);

	// если тип слова не определет, сравниваем со всеми
	if(word.type == w_default)
	{

	}
}

} /* namespace patterns */
