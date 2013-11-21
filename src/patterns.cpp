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
#include "classes/control/CControl.h"

#define PROGRAMNAME "text_patterns"

using namespace std;

void help()
{
	cout << "Usage: " << endl << PROGRAMNAME << " [-k] -c config_file_name -o output_file_name"
			<< endl;
}

bool configTest()
{
	patterns::CConfig config;
	config.loadConfig("/home/generall/temp/conf.txt");
	if (config.getValue("dir") == "ololo" && config.getValue("ololo") == "")
	{
		return true;
	}
	return false;
}

/*
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
 */
bool loadTextTest()
{
	patterns::CText asimov;
	asimov.loadFromMytsem("/home/generall/", "asimov_decoded.txt");
	asimov.performStatistics();
	return true;
}

/*
 bool loadTextXMLTest()
 {
 patterns::CText xml_tree;
 xml_tree.setStoplist(patterns::root, patterns::stoplist);
 xml_tree.loadFromXml("/home/generall/temp/", "tree.xml");
 for (int i = 0; i < 15; i++)
 {
 cout << xml_tree.text[i]->value << endl;
 }
 return true;
 }
 */

bool learningTest2(const string &group, int hard)
{
	cout << "loading learning test №2" << endl;
	patterns::CSamples s;
	//s.loadFromFiles(patterns::root, patterns::stoplist, false, true);

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
	//s.loadFromFiles(patterns::root, patterns::stoplist, false, true);
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

	std::vector<std::vector<uint> > converted;

	for (uint i = 0; i < R.size(); i++)
	{
		converted.push_back(std::vector<uint>());
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

bool hyperspaceTest()
{
	cout << "Loading hyperspace test" << endl;

	patterns::CConfig config;

	config.loadConfig("/home/generall/Dropbox/code/Ruby/habraloader/config.txt"); //загружать из argv

	bool use_xml = true;
	use_xml = config.getValue("USE_XML") != "NO";

	auto clusters = config.getClassVector();

	auto stoplist = config.getValue("STOP_LIST");

	//DEBUG
	cout << use_xml << endl;
	cout << config.getValue("TO_CLASSIFY") << endl;
	for (auto x : clusters)
	{
		cout << x << endl;
	}
	//debug

	patterns::CSamples s;
	for (auto x : clusters)
	{
		s.loadFromFiles(config.getValue(x), stoplist, x, false, true, use_xml);
	}
	s.init();

	patterns::CSamples test_texts;
	test_texts.loadFromFiles(config.getValue("TO_CLASSIFY"), stoplist, "test", false, true,
			use_xml);

	//patterns::CEuclideanDistance euclide;
	//patterns::CNaiveBayes bayes;
	//patterns::CMahlanobisDistance mahlanobis;
	patterns::CAngle angle;

	/*
	 s.createHypeespaceWithComplex(false, true);
	 cout << "Hyperspace created. Dimention: " << s.signatures.size() << endl;
	 s.deleteInsignificantDimensions(0.01);
	 cout << "Hyperspace cleaned. Dimention: " << s.signatures.size() << endl;

	 double efficiency = patterns::testClassifier(s, test_texts, &bayes);
	 cout << "min_supply = " << s.min_supply << " efficiency " << efficiency << endl;

	 for (int sup = 30; sup < 80; sup += 5)
	 {
	 s.min_supply = sup;
	 s.createHypeespaceWithComplex(false, true);
	 cout << "Hyperspace created. Dimention: " << s.signatures.size() << endl;
	 s.deleteInsignificantDimensions(0.01);
	 cout << "Hyperspace cleaned. Dimention: " << s.signatures.size() << endl;
	 efficiency = patterns::testClassifier(s, test_texts, &bayes);
	 cout << "min_supply = " << s.min_supply << " efficiency " << efficiency << endl;
	 }
	 */

	//s.min_supply = 50;
	//s.createHyperspaceWithComplex(false, true);
	//s.createWeightedWordHyperspace();
	//s.createWeightedDispersion();
	s.createHyperspaceWordsOnly();
	//s.createBinaryDispersion();

	cout << "Hyperspace created. Dimention: " << s.signatures.size() << endl;

	executeClassifier(s, test_texts, &angle, "/home/generall/temp/class_out.txt");
	//s.deleteInsignificantDimensions(0.1);
	//cout << "Hyperspace cleaned. Dimention: " << s.signatures.size() << endl;
	//double efficiency = patterns::testClassifier(s, test_texts, &angle);

	//cout << "efficiency silly normal Words " << efficiency << endl;
	//binary: 0.788732
	//nonbin: 0.760563 <--- no wonder on this planet

	//Mahlanobis
	//Бинарно:
	// С нормализацией: 64%
	// без нормализации: 64.7%
	//Признак с учетом кол-ва слов:
	// С нормализацией: 64.7%
	// Без нормализации: 61.2%

	//0.788732

	/*
	 std::vector<double> point;
	 s.createTextHyperPoint(test_texts.samples["algo"][0], point);
	 cout << "point for samples[\"algo\"][0]  created" << endl;
	 for (auto x : s.hyper_points)
	 {
	 std::cout << "distanation to " << x.first << " eq " << bayes.compare(x.second, point)
	 << endl;
	 }
	 */

	return true;
}

int main(int argc, char *agrv[])
{
	patterns::CControl ctr;

	try
	{
		ctr.init(argc, agrv);
		ctr.execute();
	} catch (logic_error & e)
	{
		cout << e.what() << endl;
		help();
	}

	/*
	 if (configTest())
	 {
	 cout << "win" << endl;
	 }
	 else
	 {
	 cout << "fail" << endl;
	 }

	 loadTextXMLTest();

	 hyperspaceTest();
	 */
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

	//FPTreeTest("coding", 20);
	return 0;
}
