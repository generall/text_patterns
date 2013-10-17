/*
 * CSamples.cpp
 *
 *  Created on: 15 окт. 2013 г.
 *      Author: generall
 */

#include "CSamples.h"

namespace patterns
{

CSamples::CSamples()
{
	// TODO Auto-generated constructor stub

}

void CSamples::loadFromFiles(std::string dir, bool has_puncluation,
		bool calcStatistics)
{

	//загрузить список из flist.txt
	std::fstream flist((dir + "/flist.txt").c_str(), std::ios::in);
	if (flist.good())
	{
		while (!flist.eof())
		{

			//fix problem with newline at the end
			std::string filename, classter;
			std::getline(flist, filename, ' ');
			std::getline(flist, classter);
			CText *t = new CText();
			t->loadFromMytsem(dir + "/" + filename, has_puncluation);
			if (calcStatistics)
				t->performStatistics();
			samples[classter].push_back(t);
			//std::cout << "loaded " << filename << std::endl;
		}
	}
	flist.close();
}

void CSamples::summStatistics(std::map<CWord*, int, CWordCompare>& s1,
		const std::map<CWord*, int, CWordCompare>& s2)
{
	for (auto x : s2)
	{
		s1[x.first] += x.second;
	}
}

void CSamples::calcGroupStat()
{
	for (auto x : samples)
	{
		std::map<CWord*, int, CWordCompare> s;
		for (auto y : x.second)
		{
			summStatistics(s, y->statistics);
		}
		std::vector<std::pair<CWord *, int>> stat_by_friquency;
		for (auto y : s)
		{
			stat_by_friquency.push_back(std::make_pair(y.first, y.second));
		}
		std::sort(stat_by_friquency.begin(), stat_by_friquency.end(),
				StatByFriquencyCmp());

		std::cout << "---------" << x.first << "-------" << std::endl;
		for (int i = 0; i < 15; i++)
		{
			std::cout << stat_by_friquency[i].first->value << "\t = "
					<< stat_by_friquency[i].second << std::endl;
		}
		statistic[x.first] = stat_by_friquency;
	}
}

void CSamples::testComplex(CPatternComplex& pattern)
{

}

CSamples::~CSamples()
{
	for (auto x : samples)
	{
		for (auto y : x.second)
		{
			delete y;
		}
	}
}

void patterns::CSamples::testPattern(CTextPattern& pattern)
{
	for (auto x : samples)
	{
		int n = 0;
		for (auto y : x.second)
		{
			n += y->testPatetrn(pattern) > 0 ? 1 : 0;
		}
		std::cout << x.first << ": " << n << " of " << x.second.size()
				<< std::endl;
	}
}

} /* namespace patterns */

