//============================================================================
// Name        : patterns.cpp
// Author      : generall
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <locale>
#include <functional>
#include "classes/pattern/CTextPattern.h"
#include "classes/pattern/CPatternComplex.h"
#include "classes/text_representation/CWord.h"
#include "classes/text_representation/CDict.h"
#include "classes/text_representation/CText.h"
#include "classes/learning/CSamples.h"
#include "classes/conffiles.h"

using namespace std;

void pause(char *label)
{
	std::cout << "pause #" << label << std::endl;
	int i = 0;
	std::cin >> i;
}

bool textPatternsTest()
{
	std::string text =
			"ololo this text is used to find some patterns patterns are hard to understand text is very hard too text is a nice pattern";
	patterns::CWord word("");
	std::stringstream text_strteam(text);
	std::vector<patterns::CToken> v_text;
	std::string temp;

	while (std::getline(text_strteam, temp, ' '))
	{
		word.value = temp;
		v_text.push_back(word);
	}

	patterns::CDelay delay;
	delay.maxDelayNumber = 5;
	patterns::CTokenPattern tokenPattern(2, "text");

	patterns::CTextPattern tp;
	auto temp_pair = std::make_pair(delay, tokenPattern);
	tp.addBack(temp_pair);

	tokenPattern.value = "pattern";
	temp_pair = std::make_pair(delay, tokenPattern);
	tp.addBack(temp_pair);

	int res = tp.compare(v_text);

	if (res == 2)
		return true;
	return false;
}

bool loadDictTest()
{

	patterns::CDict d;
	d.parseMysterm("/home/generall/ydict_test.txt");
	patterns::CWord w1("тиран");
	patterns::CWord w2("ololo");
	patterns::CWord w3("тиротрон");

	std::cout << d.nearestLevenshteinWord(w3).value << std::endl;

	//d.analysis(); pointless

	return d.findWord(w1) != -1 && d.findWord(w2) == -1;
}

bool loadTextTest()
{
	patterns::CText asimov;
	asimov.loadFromMytsem("/home/generall/", "asimov_decoded.txt");
	asimov.performStatistics();
	return true;
}

bool loadSamplesTest()
{
	patterns::CSamples s;
	s.loadFromFiles(patterns::root, patterns::stoplist, false, true);

	string recv;
	cin >> recv;
	while (recv != "exit")
	{
		patterns::CPatternComplex complex;

		string or_val;
		cin >> or_val;
		while (or_val != "break")
		{
			patterns::CDelay delay;
			patterns::CTextPattern tp;
			patterns::CTokenPattern tokenPattern(2, or_val);

			delay.maxDelayNumber = 0;
			tokenPattern.typeOfMatching = patterns::m_full;
			auto temp_pair = std::make_pair(delay, tokenPattern);
			tp.addBack(temp_pair);
			std::vector<patterns::CTextPattern> impl;
			impl.push_back(tp);
			complex.DNF.push_back(impl);
			cin >> or_val;
		}
		s.testPattern(complex);
		cin >> recv;
	}

	s.calcGroupStat();

	if (s.samples.size() == 5)
	{
		std::cout << "sample true" << std::endl;
		return true;
	}
	else
	{
		std::cout << "sample fails" << std::endl;
		return false;
	}
}

bool learningTest2(const string &group, int hard)
{
	cout << "loading learning test №2" << endl;
	patterns::CSamples s;
	s.loadFromFiles(patterns::root, patterns::stoplist, false, true);
	cout << "files loaded" << endl;
	auto v = s.generateCovers(group, hard);
	cout << "covers generated!" << endl;
	auto best_cover = s.getBestCover(v);
	for (auto x : best_cover)
	{
		cout << s.global_statistic[x].first->value << endl;
	}

	return true;
}

int main()
{
	//learningTest2("gadgets",6);

	learningTest2("coding", 5);

	if (textPatternsTest() // && loadDictTest() && loadTextTest()
	&& loadSamplesTest())
	{
		cout << "win" << endl;
	}
	else
	{
		cout << "fail" << endl;
	}

	std::regex r("ol.*");
	cout << patterns::levenshtein_distance(string("troll"), string("trall")) << endl;
	cout << std::regex_match("ololo", r) << endl;
	cout << std::regex_match("trololo", r) << endl;
	return 0;
}
