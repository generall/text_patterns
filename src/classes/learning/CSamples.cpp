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
	if (debug)
		std::cout << "calcGroupStat" << std::endl;

	global_statistic.clear();
	global_statistic_by_word.clear();
	statistic.clear();
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
		summStatistics(global_statistic_by_word, s);
	}

	for (auto y : global_statistic_by_word)
	{
		global_statistic.push_back(std::make_pair(y.first, y.second));
	}

	std::sort(global_statistic.begin(), global_statistic.end(), StatByFriquencyCmp());

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

	/*
	 correctErase();
	 signature_matrix_by_sign.clear();
	 signature_matrix_by_text.clear();
	 */

	//генерируем глобальные сигнатуры по словам
	for (auto word_sign : global_statistic)
	{
		signatures.push_back(new CWordSign(word_sign.first));
	}

	if (debug)
		std::cout << "signatures.size: " << signatures.size() << std::endl;

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
			for (auto sign : text.second)
			{
				temp.push_back(sign.first);
			}
			std::sort(temp.begin(), temp.end()); // best case expected
			group_signature_matrix_by_text_sorted[cluster.first][text.first] = temp;
		}
	}
}

void CSamples::createFPTree()
{
	for (auto cluster : group_signature_matrix_by_text_sorted)
	{
		for (auto text : cluster.second)
		{
			for (uint sign : text.second)
			{
				FPtree[cluster.first].FPAdd(sign);
			}
			FPtree[cluster.first].switchToRoot();
		}
	}
}

int CSamples::getSignature(const std::string &cluster, uint text, uint sign)
{
	auto matrix = &signature_matrix_by_text[cluster];
	auto i = matrix->find(text);
	if (i != matrix->end())
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

bool CSamples::nextCombination(std::vector<uint>& a, int n)
{
	uint k = (uint) a.size();
	for (uint i = k - 1; i >= 0; --i)
		if (a[i] < n - k + i + 1)
		{
			++a[i];
			for (uint j = i + 1; j < k; ++j)
				a[j] = a[j - 1] + 1;
			return true;
		}
	return false;
}

std::vector<std::vector<uint> > CSamples::generateCovers(const std::string &cluster, uint maxlen)
{
	createMatrix();

	std::cout << "matrix creating complete" << std::endl;

	uint n_doc = samples[cluster].size();
	std::vector<uint> combination, prev_comb;
	std::vector<std::vector<uint> > result;
	std::vector<uint> largest_cover;
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

			/*
			 std::cout << "---------#" << i << "#----------" << std::endl;
			 for (auto d : combination)
			 std::cout << " " << d;
			 std::cout << std::endl;
			 int temp;
			 std::cin >> temp;
			 */
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
				result.push_back(
						std::vector<uint>(combination.begin(), combination.begin() + i + 1));
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

double CSamples::testCover(const std::string& cluster, const std::vector<uint>& complex)
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

double CSamples::testCoverAnd(const std::string& cluster, const std::vector<uint>& complex)
{

	//переписать на более эффективную, выделять сначала все, которые подошли, потом полько на них накладывать маску
	uint total_count = 0;
	for (auto x : samples)
		total_count += x.second.size();

	std::vector<std::pair<uint, int>> last_res;
	for (uint i = 0; i < samples[cluster].size(); i++)
	{
		last_res.push_back(std::make_pair(i, 1));
	}

	for (auto sign : complex)
	{
		std::vector<std::pair<uint, int>> res;
		auto texts = &signature_matrix_by_sign[cluster][sign];
		//std::cout << "set_intersection" << std::endl;
		std::set_intersection(texts->begin(), texts->end(), last_res.begin(), last_res.end(),
				std::back_inserter(res), CPairComparator());
		last_res = res;
	}

	return (double) last_res.size() / (double) samples[cluster].size();
}
double CSamples::testCoverAnd(const std::string& cluster, const std::vector<uint>& complex,
		std::vector<uint>& covered)
{

	//переписать на более эффективную, выделять сначала все, которые подошли, потом полько на них накладывать маску
	uint total_count = 0;
	for (auto x : samples)
		total_count += x.second.size();

	std::vector<std::pair<uint, int>> last_res;
	for (uint i = 0; i < samples[cluster].size(); i++)
	{
		last_res.push_back(std::make_pair(i, 1));
	}

	for (auto sign : complex)
	{
		std::vector<std::pair<uint, int>> res;
		auto texts = &signature_matrix_by_sign[cluster][sign];
		//std::cout << "set_intersection" << std::endl;
		std::set_intersection(texts->begin(), texts->end(), last_res.begin(), last_res.end(),
				std::back_inserter(res), CPairComparator());
		last_res = res;
	}
	covered.clear();
	for (auto x : last_res)
	{
		covered.push_back(x.first);
	}

	//+++++++++++++++++++++DEBUG
	/*
	 if (cluster == "infosecurity")
	 {
	 std::cout << "section ------" << cluster << "-------" << std::endl;
	 std::cout << "matching: " << last_res.size() << " of " << samples[cluster].size()
	 << std::endl;
	 std::cout << "result: " << (double) last_res.size() / (double) samples[cluster].size()
	 << std::endl;
	 for (auto x : complex)
	 {

	 std::cout << "num: " << x << std::endl;
	 std::cout << global_statistic[x].first->value << std::endl;
	 }

	 int tmp1;
	 std::cin >> tmp1;
	 }
	 */
	//++++++++++++++++++++++DEBUG
	return (double) last_res.size() / (double) samples[cluster].size();
}

std::vector<uint> CSamples::getBestCover(std::vector<std::vector<uint> >& covers)
{
	double best_entropy = 10005000;
	uint best_complex = 0;
	int i = 0;
	for (auto c : covers)
	{
		std::vector<double> prob;

		std::cout << "________________vector: ";
		for (auto z : c)
			std::cout << z << " ";
		std::cout << std::endl;

		for (auto x : samples)
		{
			prob.push_back(testCover(x.first, c));
			std::cout << "probability #" << x.first << " = " << prob[prob.size() - 1] << std::endl;

		}
		double current_entropy = entropy(prob);
		std::cout << "Entropy #" << i << " " << current_entropy << std::endl;
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

	std::cout << "Entropy #" << " " << best_entropy << std::endl;
	return covers[best_complex];
}

void CSamples::FPFind(FPTree<uint>& tree, int delta_min, std::vector<uint> phi,
		std::vector<std::vector<uint> > &R, bool max_only)
{
	bool is_finish = true;
	if (tree.pointers.size() == 0)
		return;
	auto i = --tree.pointers.end();
	while (i != tree.pointers.begin())
	{
		if (i->second.second >= delta_min)
		{
			is_finish = false;

			//====DEBUG=====

			/*
			 std::cout << std::string(phi.size(), '>') << "create new tree with "
			 << statistic["algo"][i->first].first->value << "# " << i->first << " sup: "
			 << i->second.second << std::endl;
			 */
			//++++DEBUG+++++
			std::vector<uint> phi_n = phi;
			phi_n.push_back(i->first);
			if (!max_only)
				R.push_back(phi_n);
			FPTree<uint> CFPTree(tree, i->first); //строим условное дерево
			FPFind(CFPTree, delta_min, phi_n, R, max_only);
		}
		--i;
	}
	if (max_only && is_finish)
		R.push_back(phi);
}

std::vector<std::vector<uint> > CSamples::FPGrowth(const std::string& cluster, int delta_min,
		bool max_only)
{
	std::vector<std::vector<uint> > res;
	FPFind(FPtree[cluster], delta_min, std::vector<uint>(), res, max_only);
	return res;
}

uint CSamples::groupToGlobal(uint index, const std::string& cluster)
{
	if (agregator[cluster].size() == 0)
	{
		createAgregator();
	}
	return agregator[cluster][index];
}

void CSamples::createAgregator()
{
	//построить карту, обратную глобальной статистике
	std::map<CWord *, uint, CWordCompare> temp;
	for (uint i = 0; i < global_statistic.size(); i++)
	{
		temp[global_statistic[i].first] = i;
	}
	for (auto cluster : statistic)
	{
		for (uint i = 0; i < cluster.second.size(); i++)
		{
			agregator[cluster.first].push_back(temp[cluster.second[i].first]);
		}
	}
}

std::vector<uint> CSamples::getBestCoverAnd(std::vector<std::vector<uint> >& covers)
{
	double best_entropy = 10005000;
	uint best_complex = 0;
	int i = 0;
	for (auto c : covers)
	{
		std::vector<double> prob;

		/*
		 std::cout << "________________vector: ";
		 for (auto z : c)
		 std::cout << z << " ";
		 std::cout << std::endl;
		 */

		for (auto x : samples)
		{
			prob.push_back(testCoverAnd(x.first, c));
			//std::cout << "probability #" << x.first << " = " << prob[prob.size() - 1] << std::endl;

		}
		double current_entropy = entropy(prob);
		std::cout << "Entropy #" << i << " " << current_entropy << std::endl;
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
		prob.push_back(testCoverAnd(x.first, covers[best_complex]));
		std::cout << "vector: ";
		for (auto z : covers[best_complex])
			std::cout << z << " ";
		std::cout << std::endl;
		std::cout << "probability #" << x.first << " = " << prob[prob.size() - 1] << std::endl;
	}

	std::cout << "Entropy #" << " " << best_entropy << std::endl;
	return covers[best_complex];
}

void CSamples::createHyperspaceWordsOnly()
{
	hyper_points.clear();
	for (auto cluster : samples)
	{
		//
		for (uint i = 0; i < signatures.size(); i++)
		{
			auto matrix = &signature_matrix_by_sign[cluster.first];

			auto iter = matrix->find(i);
			if (iter == matrix->end())
			{
				hyper_points[cluster.first].push_back(0.0);
			}
			else
			{
				hyper_points[cluster.first].push_back(
						(double) iter->second.size() / (double) cluster.second.size());
			}
		}
	}
}

void CSamples::createHyperspaceWithComplex(bool with_words, bool max_only)
{
	//получить комплексы со всех класстеров с различной поддержкой
	//все комплексы конвертировать в глобальное представление с помощью agregator
	//добавить сигнатуры в signatures
	//посчитать значение покрытия и добавить в hyperspace
	//как вариант, добавлять комплексы последовательно, кластер за кластером
	//для каждого класса просчитать покрытие
	if (!with_words)
	{
		for (auto x : signatures)
		{
			delete x;
		}
		for (auto x : group_signatures)
		{
			for (auto y : x.second)
			{
				delete y;
			}
		}
		signatures.clear();
		group_signatures.clear();
		hyper_points.clear();
	}
	double ready = 0;
	for (auto x : samples)
	{
		//std::cout << "Start FPGrowth with min_supply: " << min_supply << std::endl;

		auto R = FPGrowth(x.first, min_supply, max_only);
		ready += 1.0 / (double) samples.size();

		/*
		 if (debug)
		 std::cout << "Ready: " << ready << std::endl;
		 */

		for (auto complex : R)
		{

			groupToGlobal(complex, x.first);

			std::vector<CWord *> temp;
			for (auto sign : complex)
			{
				temp.push_back(global_statistic[sign].first);
			}

			CComplexAndSing *sing_complex = new CComplexAndSing(temp);
			signatures.push_back(sing_complex);
			//uint sign_index = signatures.size() - 1;

			//Добавление сигнатур по группам, не уверен что оно вообще нужно.
			//CComplexAndSing *group_sign_complex = new CComplexAndSing(temp);
			//group_signatures[x.first].push_back(group_sign_complex);
			//uint group_sign_index = group_signatures[x.first].size() - 1;

			for (auto y : samples)
			{
				std::vector<uint> covered;
				double probability = testCoverAnd(y.first, complex, covered);
				hyper_points[y.first].push_back(probability);
			}
			//добавление признаков в signature_matrix и group_signature_matrix
			/*
			 if (with_words)
			 {
			 for (auto y : covered)
			 {
			 signature_matrix_by_sign[x.first][sign_index][y] = 1;
			 signature_matrix_by_text[x.first][y][sign_index] = 1;
			 group_signature_matrix_by_sign[x.first][group_sign_index][y] = 1;
			 group_signature_matrix_by_text[x.first][y][group_sign_index] = 1;
			 }
			 }
			 */
		}
	}
}

void CSamples::groupToGlobal(std::vector<uint>& signs, const std::string& cluster)
{
	for (uint i = 0; i < signs.size(); i++)
	{
		signs[i] = groupToGlobal(signs[i], cluster);
	}
}

void CSamples::init()
{
	createMatrix();
	std::cout << "Matrix generated" << std::endl;
	createGroupMatrix();
	std::cout << "Group matrix generated" << std::endl;
	createSortedMatrix();
	std::cout << "Sorted matrix generated" << std::endl;
	createFPTree();
	std::cout << "FPTree matrix generated" << std::endl;
	createAgregator();
	std::cout << "Agregator generated" << std::endl;
}

void CSamples::deleteInsignificantDimensions(double factor)
{
	//TODO: добавить удаление повторяющихся признаков.
	//Гипотера 1.
	//Если дисперсия (на самом деле среднеквадратичное отклонение) маленькая - то ктритерий плохой.
	//Удалять из signatures и hyper_points
	std::vector<bool> todelete;
	for (uint i = 0; i < signatures.size(); i++)
	{
		double summ = 0;
		for (auto cluster : hyper_points)
		{
			summ += cluster.second[i];
		}
		double expectation = summ / (double) hyper_points.size();
		double dispersion = 0;
		for (auto cluster : hyper_points)
		{
			dispersion += (cluster.second[i] - expectation) * (cluster.second[i] - expectation);
		}
		dispersion = dispersion / (double) hyper_points.size();
		double standart_deviation = sqrt(dispersion);
		if (standart_deviation < factor)
		{
			todelete.push_back(true);
		}
		else
		{
			todelete.push_back(false);
		}
	}
	//real delete here

	std::vector<TSignature *> new_signatures;
	for (uint i = 0; i < todelete.size(); i++)
	{
		if (!todelete[i])
		{
			new_signatures.push_back(signatures[i]);
		}
		else
		{
			delete signatures[i];
		}
	}
	signatures = new_signatures;
	for (auto cluster : hyper_points)
	{
		std::vector<double> new_point;
		for (uint i = 0; i < todelete.size(); i++)
		{
			if (!todelete[i])
			{
				new_point.push_back(cluster.second[i]);
			}
		}
		hyper_points[cluster.first] = new_point;
	}
}

void CSamples::createTextHyperPoint(CText* text, std::vector<double>& hyper_point)
{
	hyper_point.clear();
	for (uint i = 0; i < signatures.size(); i++)
	{
		uint result = signatures[i]->test(text);
		hyper_point.push_back(result);	//nonbynary, но старые сигнатуры дадут бинарные данные

		/*
		 if (result >= 1)	//Bynary hypothesis
		 {
		 hyper_point.push_back(1.0);
		 }
		 else
		 {
		 hyper_point.push_back(0.0);
		 }
		 */
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
//удаление сигнатур
	for (auto x : signatures)
	{	//
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

void CSamples::createWeightedWordHyperspace()
{
	uint size = signatures.size();

	std::vector<TSignature *> new_sign; //пересоздание вектора сигнатур
	for (auto x : signatures)
	{
		new_sign.push_back(new CWeightWordSign(dynamic_cast<CWordSign *>(x)->word));
	}
	correctErase();
	signatures = new_sign;

	hyper_points.clear();
	for (auto cluster : samples)
	{
		//
		for (uint i = 0; i < size; i++)
		{
			double summ = 0;
			auto matrix = &signature_matrix_by_sign[cluster.first];
			auto iter = matrix->find(i);
			if (iter == matrix->end())
			{
				//признака не существует -> мат. ожидание = 0;
				hyper_points[cluster.first].push_back(0.0);
			}
			else
			{
				for (auto text : iter->second)
				{
					summ += text.second;
				}
				hyper_points[cluster.first].push_back(summ / (double) cluster.second.size());
			}
		}
	}
}

void CSamples::correctErase()
{
	//удаление сигнатур
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
	signatures.clear();
	group_signatures.clear();
}

void CSamples::createBinaryDispersion()
{
	for (auto clusters : hyper_points)
	{
		for (uint i = 0; i < clusters.second.size(); i++)
		{
			double summ = 0;
			uint positive_size = signature_matrix_by_sign[clusters.first][i].size();
			uint negative_size = clusters.second.size() - positive_size;
			summ = (1.0 - clusters.second[i]) * (1.0 - clusters.second[i]) * positive_size
					+ clusters.second[i] * clusters.second[i] * negative_size;
			summ = summ / clusters.second.size();
			hyper_points_dispersion[clusters.first].push_back(summ);
		}
	}
}

} /* namespace patterns */

