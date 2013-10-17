/*
 * CComplexGenerator.cpp
 *
 *  Created on: 17 окт. 2013 г.
 *      Author: generall
 */

#include "CComplexGenerator.h"

namespace patterns
{

CComplexGenerator::CComplexGenerator()
{
	// TODO Auto-generated constructor stub

}

CPatternComplex CComplexGenerator::generatePattern(const std::string& classter, CSamples& samples,
		uint maxlen)
{
	CPatternComplex bestComplex, currentComplex;

	double best_entropy = 100500; //very large
	samples.calcGroupStat(); //initial statistics calculation
	std::vector<int> combination, prev_comb;

	std::vector<std::map<std::string, std::vector<bool> > > maskCache;

	combination.resize(maxlen);
	maskCache.resize(maxlen);
	for (int i = 0; i < maxlen; i++)
		combination[i] = i;
	prev_comb = combination;
	//добавить сюда что-то типо эвристики, например: пропускать несущественные слова
	auto globalStat = samples.statistic[classter];
	bool is_new_combination = true;
	while (is_new_combination)
	{
		for (uint i = 0; i < maxlen; i++)
		{
			if (i == 0)
			{
				currentComplex.DNF.clear(); //очистить текущий комплекс
				CTextPattern tp;
				tp.add(globalStat[combination[i]].first->value);
				//инициализировать маску первым паттерном
				samples.testPattern(tp); //все успешно прошедшие тесты инициализируются в маске как true
				maskCache[i] = samples.lastAcceptedMask; //маску в кэш
				currentComplex.add(tp); //паттерн в комплекс
			}

			//проверяем покрытие
			int delta = samples.lastPatterStatistic[classter].first
					- samples.lastPatterStatistic[classter].second;
			if (delta == 0)
			{
				//покрытие достигнуто
				//сравнить с лучшим, для этого посчитать энтропию
				std::vector<double> probability_data;
				for (auto x : samples.lastPatterStatistic)
				{
					probability_data.push_back((double) x.second.first / (double) x.second.second);
				}
				double current_entropy = entropy(probability_data);
				if (current_entropy < best_entropy)
				{				//новый кандидат обнаружен
					bestComplex = currentComplex;
					prev_comb = combination;
					is_new_combination = nextCombination(combination, max_word_to_consider);
					//удалить лишнее из кэша маски и из патерна
					int sameCount = 0;
					for (int j = 0; j < maxlen; j++)
					{
						if(prev_comb[j] == combination[j])
						{
							sameCount++;
						}else{
							break;
						}
					}
					while(currentComplex.DNF.size() > sameCount)
						currentComplex.del();
					while(maskCache.size() > sameCount)
						maskCache.erase(maskCache.end() - 1);
				}
			}
		}
	}
	return bestComplex;
}

bool CComplexGenerator::nextCombination(std::vector<int>& a, int n)
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

double CComplexGenerator::entropy(std::vector<double> &data)
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

CComplexGenerator::~CComplexGenerator()
{
	// TODO Auto-generated destructor stub
}

} /* namespace patterns */
