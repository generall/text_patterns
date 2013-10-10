//============================================================================
// Name        : patterns.cpp
// Author      : generall
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include "classes/pattern/CTextPattern.h"
#include "classes/text_representation/CWord.h"
#include "classes/text_representation/CDict.h"
using namespace std;

bool textPatternsTest()
{
	std::string text =
			"ololo this text is used to find some patterns patterns are hard to understand text is very hard too text is a nice pattern";
	patterns::CWord word("");
	std::stringstream text_strteam(text);
	std::vector<patterns::CToken> v_text;

	while (std::getline(text_strteam, word.value, ' '))
	{
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
	patterns::CWord w("яяя");

	//return false;
	std::cout << d.find_word(w) << std::endl;

	for (int i = 0; i < 80; i++)
	{
		std::cout<<i <<": " << d.dictionary_by_alpha[i]->value << std::endl;
	}

	return d.find_word(w) != -1;
}

int main()
{

	if (textPatternsTest() && loadDictTest())
	{
		cout << "win" << endl;
	}
	else
	{
		cout << "fail" << endl;
	}

	std::regex r("ol.*");
	cout << patterns::levenshtein_distance(string("ololo"), string("olkoslo"))
			<< endl;
	cout << std::regex_match("ololo", r) << endl;
	cout << std::regex_match("trololo", r) << endl;
	return 0;
}
