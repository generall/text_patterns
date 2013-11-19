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

}

void CText::setStoplist(const std::string &dir,const std::string &st_list)
{
	stoplist = st_list;
	if (stoplist.size() > 0)
		stop_list.loadSimple(dir + "/" + stoplist);

}

void CText::initStopDic(CDict& dict)
{
	stop_list = dict;
}

void CText::loadFromMytsem(const std::string &dir, const std::string &filename,
		bool has_punctuation)
{


	std::setlocale(LC_ALL, "ru_RU.UTF-8");
	std::fstream in((dir + "/" + filename).c_str(), std::ios::in);
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
				if (stop_list.findWord(first_form) == -1 && first_form.size() >= 3)
				{
					text.push_back(w);
				}
				else
				{
					delete w;
				}
			}
		}
		n++;
	}
	in.close();
	file_name = dir + "/" + filename;
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
	std::sort(stat_by_friquency.begin(), stat_by_friquency.end(), StatByFriquencyCmp());

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

void CText::loadFromXml(const std::string& dir, const std::string& filename)
{
	loadFromXml(dir + filename);
}

void CText::loadFromXml(const std::string& filename)
{
	TiXmlDocument doc(filename.c_str());
	doc.LoadFile(TIXML_ENCODING_UTF8);
	TiXmlElement *root = doc.FirstChildElement("semantrees");
	if (root)
	{
		TiXmlElement *tree = root->FirstChildElement("tree");
		while(tree)
		{
			TiXmlElement *src_form = tree->FirstChildElement("source_forms");
			if(src_form)
			{
				TiXmlElement *form = src_form->FirstChildElement("form");
				while(form)
				{
					std::string first_form(form->Attribute("srcform"));

					CWord *w = new CWord(first_form);
					w->wordType = w_default; // because atribN is not useful !
					if (stop_list.findWord(first_form) == -1 && first_form.size() >= 3)
					{
						text.push_back(w);
					}
					form = form->NextSiblingElement("form");

				}
			}

			tree = tree->NextSiblingElement("tree");
		}
	}
	else
	{
		throw std::logic_error("Incorrect XML");
	}
	file_name = filename;
}


} /* namespace patterns */


