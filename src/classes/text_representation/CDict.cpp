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

	std::sort(dictionary_by_length.begin(), dictionary_by_length.end(),
			CWordCompareLength());
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
		std::string first_form, get_form, type;
		std::smatch m;
		std::stringstream text_strteam(line);

		std::getline(text_strteam, get_form, ' ');
		std::getline(text_strteam, first_form, ' ');
		std::getline(text_strteam, type);

		//std::cout << "1: " << get_form << " 2: " << first_form << " 3: " << type
		//		<< std::endl;


		//std::wchar_t *buffer = new std::wchar_t[first_form.size()];
		//iconv(cd, first_form.size(), first_form.c_str(), first_form.size(), buffer);



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

void CDict::analysis()
{

	if (!reorganised)
		reorganiseToWork();

	std::vector<uint> size_dist_tab;
	size_dist_tab.resize(100, 100);
	for (uint i = 0; i < dictionary_by_length.size(); i++)
	{
		if(i%1000 == 0)
			std::cout<< i <<" of " << dictionary_by_length.size() <<std::endl;

		auto leni = dictionary_by_length[i]->value.length();
		for (uint j = i + 1; j < dictionary_by_length.size(); j++)
		{
			auto lenj = dictionary_by_length[j]->value.length();
			if ((lenj - leni) > size_dist_tab[leni])
			{
				break;
			}
			if (dictionary_by_length[j]->wordType
					== dictionary_by_length[i]->wordType
					|| dictionary_by_length[i]->wordType == w_default)
			{
				auto dist = levenshtein_distance(dictionary_by_length[j]->value,
						dictionary_by_length[i]->value);
				size_dist_tab[leni] = std::min(size_dist_tab[leni], dist);
				size_dist_tab[lenj] = std::min(size_dist_tab[lenj], dist);
			}
		}
	}
	for (uint i = 0; i < size_dist_tab.size(); i++)
	{
		std::cout << i << ":\t " << size_dist_tab[i] << std::endl;
	}
}

void CDict::loadSimple(const std::string& filename)
{
	std::setlocale(LC_ALL, "en_US.UTF-8");
	std::fstream in(filename.c_str(), std::ios::in);
	char line[1024];

	while (!in.eof())
	{
		in.getline(line, 1024);
		std::string first_form(line);
		CWord w(first_form);
		w.wordType = w_default;
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

	if (!reorganised)
		reorganiseToWork();

	auto t = word.value.length();
	t = 1;

	uint max_dist = getMaxLevenshteinDist(word.value);
	auto lower_length_bound = std::max(word.value.length() - max_dist, t);
	auto upper_length_bound = word.value.length() + max_dist;

	CWord lower_bound_word(std::string(lower_length_bound, 'a'));
	CWord upper_bound_word(std::string(upper_length_bound, 'a'));

	std::vector<CWord*>::iterator lower_iterator = std::lower_bound(
			dictionary_by_length.begin(), dictionary_by_length.end(),
			&lower_bound_word, CWordCompareLength());
	std::vector<CWord*>::iterator upper_iterator = std::upper_bound(
			dictionary_by_length.begin(), dictionary_by_length.end(),
			&upper_bound_word, CWordCompareLength());

	/*
	 std::cout << "len: "<< word.value.length() - max_dist <<std::endl;
	 std::cout << "len: "<< dictionary_by_length.size()<<std::endl;
	 std::cout << "len: "<< dictionary_by_length[1]->value.length() <<std::endl;

	 std::cout << "low: "<< lower_bound_word.value <<std::endl;
	 std::cout << "upp: "<< upper_bound_word.value <<std::endl;
	 std::cout << "low: "<< (*lower_iterator)->value <<std::endl;
	 std::cout << "upp: "<< (*upper_iterator)->value <<std::endl;
	 */

	// если тип слова не определет, сравниваем со всеми
	CWord *nearestWord = &word;
	uint min_dist = max_dist;
	std::vector<CWord*>::iterator i = lower_iterator;
	if (word.wordType == w_default)
	{
		while (i != upper_iterator)
		{
			//std::cout << (*i)->value << std::endl;
			//возможно можно организовать кэш тут.
			uint dist = levenshtein_distance(word.value, (*i)->value);
			if (dist < min_dist)
			{
				min_dist = dist;
				nearestWord = (*i);
			}
			i++;
		}

	}
	else
	{
		while (i != upper_iterator)
		{
			if (word.wordType == (*i)->wordType)
			{
				//std::cout << (*i)->value << std::endl;
				//возможно можно организовать кэш тут.
				uint dist = levenshtein_distance(word.value, (*i)->value);
				if (dist < min_dist)
				{
					min_dist = dist;
					nearestWord = (*i);
				}
			}
			i++;
		}
	}
	return *nearestWord;
}

} /* namespace patterns */
