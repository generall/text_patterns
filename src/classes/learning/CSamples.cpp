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

void CSamples::loadFromFiles(std::string dir, bool has_puncluation, bool calcStatistics)
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
	lastStatistic.clear();
	lastStatisticByWord.clear();
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
		std::sort(stat_by_friquency.begin(), stat_by_friquency.end(), StatByFriquencyCmp());

		lastStatisticByWord[x.first] = s;
		//std::cout << "---------" << x.first << "-------" << std::endl;
		/*
		for (uint i = 0; i < 10; i++)
		{
			std::cout << stat_by_friquency[i].first->value << "\t = " << stat_by_friquency[i].second
					<< std::endl;
		}
		*/
		statistic[x.first] = stat_by_friquency;
	}
	lastStatistic = statistic;
}

void CSamples::calcGroupStat(std::map<std::string, std::vector<bool> > &mask, bool accept)
{
	lastStatistic.clear();
	lastStatisticByWord.clear();
	for (auto x : mask)
	{
		if (samples[x.first].size() != x.second.size())
			throw std::logic_error("wrong mask");

		std::map<CWord*, int, CWordCompare> s;

		for (uint i = 0; i < samples[x.first].size(); i++)
		{
			if (x.second[i] == accept)
			{
				summStatistics(s, samples[x.first][i]->statistics);

				std::vector<std::pair<CWord *, int>> stat_by_friquency;
				for (auto y : s)
				{
					stat_by_friquency.push_back(std::make_pair(y.first, y.second));
				}
				std::sort(stat_by_friquency.begin(), stat_by_friquency.end(), StatByFriquencyCmp());
				lastStatistic[x.first] = stat_by_friquency;
			}
		}
		lastStatisticByWord[x.first] = s;
	}
}

void CSamples::testPattern(const TPatternInterface& pattern,
		std::map<std::string, std::vector<bool> >& mask, bool accept, bool renew)
{
	lastPatterStatistic.clear();
	for (auto x : mask)
	{
		int n = 0;
		int m = 0;
		//lastAcceptedMask[x.first] = std::vector<bool>();

		auto t = samples[x.first];
		for (uint i = 0; i < t.size(); i++)
		{ //итерируемся по всем текстам

			if (x.second[i] == accept)
			{ // пропускаем те, которые запрещены маской
				uint test = t[i]->testPatetrn(pattern) > 0 ? 1 : 0;
				n += test;
				lastAcceptedMask[x.first][i] = (test > 0 ? renew : !renew); // обновляем маску (дополняем до общего, или отнимаем от общего)
				m++;
			}
		}
		if (n == 0)
		{
			//lastAcceptedMask.erase(x.first);
		}
		lastPatterStatistic[x.first] = std::make_pair(n, m);
		//std::cout << x.first << ": " << n << " of " << x.second.size() << std::endl;
	}
}

void CSamples::testPattern(const TPatternInterface& pattern)
{
	lastAcceptedMask.clear();
	lastPatterStatistic.clear();
	for (auto x : samples)
	{
		int n = 0;
		int m = 0;
		lastAcceptedMask[x.first] = std::vector<bool>();

		for (auto y : x.second)
		{
			uint test = y->testPatetrn(pattern) > 0 ? 1 : 0;
			n += test;
			lastAcceptedMask[x.first].push_back(test > 0 ? true : false);
			m++;
		}
		if (n == 0)
		{
			lastAcceptedMask.erase(x.first);
		}
		lastPatterStatistic[x.first] = std::make_pair(n, m);
		//std::cout << x.first << ": " << n << " of " << x.second.size() << std::endl;
	}
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

} /* namespace patterns */

