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

void CSamples::loadFromFiles(std::string dir, std::string stoplist, bool has_puncluation,
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

			t->stoplist = stoplist;

			t->loadFromMytsem(dir, filename, has_puncluation);
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

		statistic[x.first] = stat_by_friquency;
	}
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
	//получаем глобальную статистику
	if (statistic.size() == 0)
	{
		calcGroupStat();
	}
	//генерируем глобальные сигнатуры по словам
	for (auto word_sign : global_statistic)
	{
		signatures.push_back(new CWordSign(word_sign.first));
	}

	for (auto cluster : samples)
	{ //для каждого класстера

		auto documents = &cluster.second;
		for (uint i = 0; i < signatures.size(); i++)
		{
			for (uint j = 0; j < documents->size(); j++)
			{
				auto iter = (*documents)[j]->statistics.find(global_statistic[i].first);
				if (iter != (*documents)[j]->statistics.end())
				{
					signature_matrix_by_sign[cluster.first][i][j] = iter->second;
					signature_matrix_by_text[cluster.first][j][i] = iter->second;
				}
			}
		}
	}
}

void CSamples::createGroupMatrix()
{
	if (statistic.size() == 0)
	{
		calcGroupStat();
	}
	for (auto cluster : statistic)
	{
		for (auto word_sign : cluster.second)
		{
			group_signatures[cluster.first].push_back(new CWordSign(word_sign.first));
		}
	}

	for (auto cluster : samples)
	{ //для каждого класстера

		auto documents = &cluster.second;

		for (uint i = 0; i < group_signatures[cluster.first].size(); i++)
		{
			for (uint j = 0; j < documents->size(); j++)
			{

				auto iter = (*documents)[j]->statistics.find(statistic[cluster.first][i].first);
				if (iter != (*documents)[j]->statistics.end())
				{
					group_signature_matrix_by_sign[cluster.first][i][j] = iter->second;
					group_signature_matrix_by_text[cluster.first][j][i] = iter->second;
				}
			}
		}
	}
}

void CSamples::createSortedMatrix()
{
	for (auto cluster : group_signature_matrix_by_text)
	{
		for (auto text : cluster.second)
		{
			std::vector<uint> temp;
			for(auto sign: text.second)
			{
				temp.push_back(sign.first);
			}
			std::sort(temp.begin(),temp.end()); // best case expected
			group_signature_matrix_by_text_sorted[cluster.first][text.first] = temp;
		}
	}
}

void CSamples::createFPTree()
{
	for(auto cluster: group_signature_matrix_by_text_sorted)
	{
		for(auto text: cluster.second)
		{
			for(uint sign: text.second)
			{
				FPtree[cluster.first].FPAdd(sign );
			}
			FPtree[cluster.first].switchToRoot();
		}
	}
}




int CSamples::getSignature(const std::string &cluster, uint text, uint sign)
{
	auto matrix = signature_matrix_by_text[cluster];
	auto i = matrix.find(text);
	if (i != matrix.end())
	{
		auto j = i->second.find(sign);
		if (j != i->second.end())
		{
			return j->second;
		}
	}
	return 0;
}

double CSamples::entropy(std::vector<double>& data)
{
	double sigma = 0;
	for (auto x : data)
	{
		if (x == 0)
		{ //prevent divizionByZero
			continue;
		}
		else
		{
			sigma += x * std::log2(x);
		}
	}
	return -sigma;
}

bool CSamples::nextCombination(std::vector<int>& a, int n)
{
	int k = (int) a.size();
	for (int i = k - 1; i >= 0; --i)
		if (a[i] < n - k + i + 1)
		{
			++a[i];
			for (int j = i + 1; j < k; ++j)
				a[j] = a[j - 1] + 1;
			return true;
		}
	return false;
}

std::vector<std::vector<int> > CSamples::generateCovers(const std::string &cluster, uint maxlen)
{
	createMatrix();

	std::cout << "matrix creating complete" << std::endl;

	uint n_doc = samples[cluster].size();
	std::vector<int> combination, prev_comb;
	std::vector<std::vector<int> > result;
	std::vector<int> largest_cover;
	uint largest_cover_num = n_doc;

	combination.resize(maxlen);
	for (uint i = 0; i < maxlen; i++)
		combination[i] = i;
	prev_comb = combination;

	std::vector<std::vector<bool> > maskCache;
	std::vector<std::pair<uint, uint> > statCache;
	maskCache.resize(maxlen);
	statCache.resize(maxlen);
	for (uint i = 0; i < maxlen; i++)
	{
		maskCache[i].resize(n_doc, false);
	}

	uint already_estimated = 0;
	bool is_new_combination = true;
	bool generate_new_combination = false;

	while (is_new_combination)
	{
		/*
		 std::cout << "current combination:" << std::endl;
		 for (auto d : combination)
		 std::cout << " " << d;
		 std::cout << std::endl;
		 */
		for (uint i = already_estimated; i < maxlen; i++)
		{
			//	std::cout << "---------#" << i << "#----------" << std::endl;
			//	int temp;
			//	std::cin >> temp;
			if (i == 0)
			{
				std::cout << combination[i] << std::endl;

				for (uint j = 0; j < n_doc; j++)
					maskCache[i][j] = false;
				statCache[i].first = 0;
				statCache[i].second = n_doc;
				//итерируемся по матрице и получам непустные сигны, записываем их в маску и в стату
				auto iter = signature_matrix_by_sign[cluster].find(combination[i]);
				if (iter != signature_matrix_by_sign[cluster].end())
				{
					for (auto x : iter->second)
					{
						if (!maskCache[i][x.first])
						{
							maskCache[i][x.first] = true;
							statCache[i].first += 1;
						}
					}
				}
				else
				{
					std::cout << "combination[" << i << "] = " << combination[i] << " not found"
							<< std::endl;
					generate_new_combination = true;
				}
			}
			else
			{
				statCache[i].first = 0;
				statCache[i].second = statCache[i - 1].second - statCache[i - 1].first;
				for (uint j = 0; j < n_doc; j++)
					maskCache[i][j] = maskCache[i - 1][j];

				auto iter = signature_matrix_by_sign[cluster].find(combination[i]);
				if (iter != signature_matrix_by_sign[cluster].end())
				{
					for (auto x : iter->second)
					{
						if (!maskCache[i][x.first])
						{
							maskCache[i][x.first] = true;
							statCache[i].first += 1;
						}
					}
				}
				else
				{
					generate_new_combination = true;
				}
			}
			//===================================================================================

			uint delta = statCache[i].second - statCache[i].first;
			if (delta == 0 && !generate_new_combination)
			{
				//std::cout << "cover is reached" << std::endl;
				result.push_back(std::vector<int>(combination.begin(), combination.begin() + i));
				generate_new_combination = true;
			}
			//===================================================================================
			if (i == maxlen - 1)
			{
				if (delta < largest_cover_num)
				{
					largest_cover = combination;
					largest_cover_num = delta;
				}
				generate_new_combination = true;
			}

			if (generate_new_combination)
			{
				generate_new_combination = false;
				//-------------------------------------
				bool is_valid = false;
				uint sameCount = 0;
				//прокручиваем лишние комбинации
				while (!is_valid)
				{
					prev_comb = combination;
					is_new_combination = nextCombination(combination, max_word_to_consider);
					if (!is_new_combination)
					{
						if (result.size() == 0)
							result.push_back(largest_cover);
						return result;
					}
					sameCount = 0;
					for (uint j = 0; j <= i; j++)
					{
						if (prev_comb[j] == combination[j])
						{
							sameCount++;
						}
						else
						{
							break;
						}
					}
					if (sameCount == i + 1)
					{
						is_valid = false;
					}
					else
					{
						is_valid = true;
					}
				}
				/*
				 for (auto d : combination)
				 std::cout << " " << d;
				 std::cout << std::endl;
				 */
				already_estimated = sameCount;
				break;
				//-------------------------------------
			}
		}
	}
	if (result.size() == 0)
		result.push_back(largest_cover);
	return result;
}

double CSamples::testCover(const std::string& cluster, const std::vector<int>& complex)
{
	std::vector<bool> mask;
	mask.resize(samples[cluster].size(), false);
	uint total_count = 0;
	for (auto x : samples)
		total_count += x.second.size();

	int n = 0;
	for (auto x : complex)
	{
		auto iter = signature_matrix_by_sign[cluster].find(x);
		if (iter != signature_matrix_by_sign[cluster].end())
		{
			for (auto y : iter->second)
			{
				if (!mask[y.first])
				{
					mask[y.first] = true;
					n += 1;
				}
			}
		}
	}
	return (double) n / (double) total_count;
}

std::vector<int> CSamples::getBestCover(std::vector<std::vector<int> >& covers)
{
	double best_entropy = 10005000;
	uint best_complex = 0;
	int i = 0;
	for (auto c : covers)
	{
		std::vector<double> prob;
		for (auto x : samples)
		{
			prob.push_back(testCover(x.first, c));
			/*
			 std::cout<<"vector: ";
			 for (auto z : c)
			 std::cout << z << " ";
			 std::cout << std::endl;
			 std::cout << "probability #" << x.first << " = " << prob[prob.size() - 1] << std::endl;
			 */
		}
		double current_entropy = entropy(prob);
		//std::cout << "Entropy #" << i <<current_entropy <<std::endl;
		//std::cout << "---------------------------------" <<std::endl;
		if (current_entropy < best_entropy)
		{
			best_entropy = current_entropy;
			best_complex = i;
		}
		i++;
	}
	std::vector<double> prob;
	for (auto x : samples)
	{
		prob.push_back(testCover(x.first, covers[best_complex]));
		std::cout << "vector: ";
		for (auto z : covers[best_complex])
			std::cout << z << " ";
		std::cout << std::endl;
		std::cout << "probability #" << x.first << " = " << prob[prob.size() - 1] << std::endl;
	}

	double current_entropy = entropy(prob);
	std::cout << "Entropy #" << i << current_entropy << std::endl;
	return covers[best_complex];
}

void CSamples::FPFind(FPTree<uint>& tree, int delta_min, std::vector<uint>& phi,
		std::vector<std::vector<uint> > &R)
{
	auto i = --tree.pointers.end();

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
	for (auto x : signatures)
	{
		delete x;
	}
//удаление груповых сигнатур
	for (auto x : group_signatures)
	{
		for (auto y : x.second)
		{
			delete y;
		}
	}
}

} /* namespace patterns */


