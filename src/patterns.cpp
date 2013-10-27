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
	for (auto x : s.samples)
	{
		cout << x.first << " - " << x.second.size() << endl;
	}
	auto v = s.generateCovers(group, hard);

	cout << "covers generated!" << endl;
	auto best_cover = s.getBestCover(v);
	for (auto x : best_cover)
	{
		cout << s.global_statistic[x].first->value << endl;
	}

	std::cout << "global_statistic.size() = " << s.global_statistic.size() << std::endl;

	return true;
}

bool FPTreeTest(const string &group, uint hard)
{
	cout << "loading FPTreeTest" << endl;
	patterns::CSamples s;
	s.loadFromFiles(patterns::root, patterns::stoplist, false, true);
	cout << "files loaded" << endl;
	s.createMatrix();
	cout << "Matrix generateg" << endl;
	s.createGroupMatrix();
	cout << "Group matrix generateg" << endl;
	s.createSortedMatrix();
	cout << "Sorted matrix generated" << endl;
	s.createFPTree();
	cout << "FPTree matrix generated" << endl;
	s.createAgregator();
	cout << "Agregator generated" << endl;

	//s.FPtree["algo"].print(s.FPtree["algo"].root, 0);
	std::cout << s.FPtree[group].pointers.size() << " <-- tree uniq word size" << std::endl;

	/*
	 for (auto x : s.group_signature_matrix_by_text_sorted["algo"])
	 {

	 std::cout << "-------------" << x.first << "--------------" << std::endl;
	 for (auto y : x.second)
	 {
	 std::cout << y<<"# " << s.statistic["algo"][y].first->value << std::endl;
	 }
	 }
	 */

	/*
	 for(auto x: s.statistic["algo"])
	 {
	 std::cout << x.first->value << " : " << x.second <<std::endl;
	 }
	 */

	auto R = s.FPGrowth(group, hard);

	std::cout << "grop founded: " << R.size() << std::endl;

	std::vector<std::vector<int> > converted;

	for (uint i = 0; i < R.size(); i++)
	{
		converted.push_back(std::vector<int>());
		for (uint j = 0; j < R[i].size(); j++)
		{
			converted[i].push_back(s.groupToGlobal(R[i][j], group));
		}
	}
	auto res = s.getBestCoverAnd(converted);
	for (int i : res)
	{
		cout << s.global_statistic[i].first->value << endl;
	}

	return true;
}

int main()
{

	//learningTest2("algo", 4);

	//learningTest2("gadgets", 3);


	/*
	 patterns::FPTree<int> tt;

	 tt.FPAdd(1);
	 tt.FPAdd(2);
	 tt.FPAdd(3);
	 tt.FPAdd(4);
	 tt.switchToRoot();
	 tt.FPAdd(1);
	 tt.FPAdd(3);
	 tt.FPAdd(4);
	 tt.switchToRoot();
	 tt.FPAdd(1);
	 tt.FPAdd(2);
	 tt.FPAdd(4);


	 tt.print(tt.root,0);
	 patterns::FPTree<int> tt2(tt, 4);
	 tt2.print(tt2.root,0);

	*/


	FPTreeTest("algo", 45);

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
