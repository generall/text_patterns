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
#include "classes/pattern/CTextPattern.h"
#include "classes/text_representation/CWord.h"
#include "classes/text_representation/CDict.h"
#include "classes/text_representation/CText.h"
#include "classes/learning/CSamples.h"

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
	patterns::CWord word(L"");
	std::stringstream text_strteam(text);
	std::vector<patterns::CToken> v_text;
	std::string temp;

	while (std::getline(text_strteam, temp, ' '))
	{
		word.value = patterns::utf8to16(temp);
		v_text.push_back(word);
	}

	patterns::CDelay delay;
	delay.maxDelayNumber = 5;
	patterns::CTokenPattern tokenPattern(2, L"text");

	patterns::CTextPattern tp;
	auto temp_pair = std::make_pair(delay, tokenPattern);
	tp.addBack(temp_pair);

	tokenPattern.value = L"pattern";
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
	patterns::CWord w1(L"тиран");
	patterns::CWord w2(L"ololo");
	patterns::CWord w3(L"тиротрон");

	std::cout << patterns::utf16to8(d.nearestLevenshteinWord(w3).value)
			<< std::endl;

	//d.analysis(); pointless

	return d.findWord(w1) != -1 && d.findWord(w2) == -1;
}

bool loadTextTest()
{
	patterns::CText asimov;
	asimov.loadFromMytsem("/home/generall/asimov_decoded.txt");
	asimov.performStatistics();
	return true;
}

bool loadSamplesTest()
{
	patterns::CSamples s;
	s.loadFromFiles("/home/generall/Dropbox/code/Ruby/habraloader", false,
			true);

	wstring recv;
	wcin >> recv;
	while (recv != L"exit")
	{
		patterns::CDelay delay;
		patterns::CTextPattern tp;
		patterns::CTokenPattern tokenPattern(2, recv.c_str());

		delay.maxDelayNumber = 0;
		tokenPattern.typeOfMatching = patterns::m_full;
		auto temp_pair = std::make_pair(delay, tokenPattern);
		tp.addBack(temp_pair);

		s.testPattern(tp);
		wcin >> recv;
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

int main()
{

	if (textPatternsTest() && loadDictTest() && loadTextTest()
			&& loadSamplesTest())
	{
		cout << "win" << endl;
	}
	else
	{
		cout << "fail" << endl;
	}

	std::wregex r(L"ol.*");
	cout << patterns::levenshtein_distance(wstring(L"troll"), wstring(L"trall"))
			<< endl;
	cout << std::regex_match(L"ololo", r) << endl;
	cout << std::regex_match(L"trololo", r) << endl;
	return 0;
}
