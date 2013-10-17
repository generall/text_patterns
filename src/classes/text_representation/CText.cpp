/*
 * CText.cpp
 *
 *  Created on: 13 окт. 2013 г.
 *      Author: generall
 */

#include "CText.h"

namespace patterns
{

CText::CText()
{
	// TODO Auto-generated constructor stub

}

void CText::loadFromMytsem(std::string filename, bool has_punctuation)
{
	std::setlocale(LC_ALL, "en_US.UTF-8");
	std::fstream in(filename.c_str(), std::ios::in);
	char line[1024];
	int n = 0;
	while (!in.eof())
	{
		in.getline(line, 1024);
		//if punctuation
		if (std::strlen(line) != 0)
		{
			if (n % 2 == 0 && has_punctuation)
			{
				CPunctuation *p = new CPunctuation();
				switch (line[0])
				{
				case ',':
					p->punctuationType = p_comma;
					break;
				case '.':
					p->punctuationType = p_dot;
					break;
				default:
					p->punctuationType = p_default;
					break;
				}
				p->value = std::string(line);
				text.push_back(p);
			}
			else
			{
				std::string first_form, type;
				std::stringstream text_strteam(line);

				std::getline(text_strteam, first_form, ' ');
				std::getline(text_strteam, type);
				std::transform(first_form.begin(), first_form.end(), first_form.begin(), ::tolower);
				CWord *w = new CWord(first_form);
				w->wordType = w_default;
				if ("S" == type)
					w->wordType = w_noun;
				if ("A" == type)
					w->wordType = w_adjective;
				if ("V" == type)
					w->wordType = w_verb;

				text.push_back(w);
			}
		}
		n++;
	}
	in.close();
}

void CText::performStatistics()
{
	for (auto x : text)
	{
		if (x->getType() == type_word)
		{
			CWord *w = dynamic_cast<CWord *>(x);
			if (statistics.find(w) != statistics.end())
			{
				statistics[w]++;
			}
			else
			{
				statistics[w] = 1;
			}
		}
	}
	for (auto x : statistics)
	{
		stat_by_friquency.push_back(std::make_pair(x.first, x.second));
	}
	std::sort(stat_by_friquency.begin(), stat_by_friquency.end(),
			StatByFriquencyCmp());

	/*
	for (int i = 0; i < 10; i++)
	{
		std::cout << stat_by_friquency[i].first->value << "\t = "
				<< stat_by_friquency[i].second << std::endl;
	}
	*/
}

CText::~CText()
{
	for (auto x : text)
	{
		delete x;
	}
}

int CText::testPatetrn(const TPatternInterface& pattern)
{
	return pattern.cmp(text);
}

} /* namespace patterns */


