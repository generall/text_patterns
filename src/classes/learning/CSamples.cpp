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
	last_statistic.clear();
	last_statistic_by_word.clear();
	global_statistic.clear();
	global_statistic_by_word.clear();
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

		last_statistic_by_word[x.first] = s;
		statistic[x.first] = stat_by_friquency;
	}
	last_statistic = statistic;
	for (auto x : last_statistic_by_word)
	{
		summStatistics(global_statistic_by_word, x.second);
	}
	for (auto y : global_statistic_by_word)
	{
		global_statistic.push_back(std::make_pair(y.first, y.second));
	}
	std::sort(global_statistic.begin(), global_statistic.end(), StatByFriquencyCmp());
}

void CSamples::calcGroupStat(std::map<std::string, std::vector<bool> > &mask, bool accept)
{
	last_statistic.clear();
	last_statistic_by_word.clear();
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
				last_statistic[x.first] = stat_by_friquency;
			}
		}
		last_statistic_by_word[x.first] = s;
	}
}

void CSamples::testPattern(const TPatternInterface& pattern,
		std::map<std::string, std::vector<bool> >& mask, bool accept, bool renew)
{
	last_patter_statistic.clear();
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
				last_accepted_mask[x.first][i] = (test > 0 ? renew : !renew); // обновляем маску (дополняем до общего, или отнимаем от общего)
				m++;
			}
		}
		if (n == 0)
		{
			//lastAcceptedMask.erase(x.first);
		}
		last_patter_statistic[x.first] = std::make_pair(n, m);
		//std::cout << x.first << ": " << n << " of " << x.second.size() << std::endl;
	}
}

void CSamples::testPattern(const TPatternInterface& pattern)
{
	last_accepted_mask.clear();
	last_patter_statistic.clear();
	for (auto x : samples)
	{
		int n = 0;
		int m = 0;
		last_accepted_mask[x.first] = std::vector<bool>();

		for (auto y : x.second)
		{
			uint test = y->testPatetrn(pattern) > 0 ? 1 : 0;
			n += test;
			last_accepted_mask[x.first].push_back(test > 0 ? true : false);
			m++;
		}
		if (n == 0)
		{
			last_accepted_mask.erase(x.first);
		}
		last_patter_statistic[x.first] = std::make_pair(n, m);
		//std::cout << x.first << ": " << n << " of " << x.second.size() << std::endl;
	}
}

void CSamples::createMatrix()
{
	//генерируем глобальные сигнатуры по словам
	for (auto wordSign : global_statistic)
	{
		signatures.push_back(new CWordSign(wordSign.first));
	}

	for (auto cluster : samples)
	{ //для каждого класстера

		auto documents = cluster.second;
		for (uint i = 0; i < signatures.size(); i++)
		{
			for (uint j = 0; j < documents.size(); j++)
			{
				auto iter = documents[j]->statistics.find(global_statistic[i].first);
				if(iter != documents[j]->statistics.end())
				{
					signature_matrix_by_sign[cluster.first][i][j] = iter->second;
					signature_matrix_by_text[cluster.first][j][i] = iter->second;
				}
			}
		}
	}
}


int CSamples::getSignature(const std::string &cluster, uint text, uint sign)
{
	auto matrix = signature_matrix_by_text[cluster];
	auto i = matrix.find(text);
	if(i != matrix.end())
	{
		auto j = i->second.find(sign);
		if(j != i->second.end())
		{
			return j->second;
		}
	}
	return 0;
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
	//добавить удаление сигнатур
	for(auto x: signatures)
	{
		delete x;
	}
}

} /* namespace patterns */

