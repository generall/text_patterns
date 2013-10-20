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
	for (uint i = 0; i < maxlen; i++)
		combination[i] = i;
	prev_comb = combination;
	//добавить сюда что-то типо эвристики, например: пропускать несущественные слова
	auto globalStat = samples.statistic[classter];
	bool is_new_combination = true;
	uint already_estimated = 0;
	bool generate_new_combination = false;
	while (is_new_combination)
	{
		std::cout << "current combination:" << std::endl;
		for (auto d : combination)
			std::cout << " " << d;
		std::cout << std::endl;

		for (uint i = already_estimated; i < maxlen; i++)
		{
			int temp;
			std::cout << "-------------------------------" << std::endl;
			//std::cout << "every position confirm required" << std::endl;
			//std::cin >> temp;

			std::cout << "now estimating: #" << i << " : "
					<< globalStat[combination[i]].first->value << std::endl;
			if (i == 0)
			{

				currentComplex.DNF.clear(); //очистить текущий комплекс
				CTextPattern tp;
				tp.add(globalStat[combination[i]].first->value);
				//инициализировать маску первым паттерном
				samples.testPattern(tp); //все успешно прошедшие тесты инициализируются в маске как true
				maskCache[i] = samples.last_accepted_mask; //маску в кэш
				currentComplex.add(tp); //паттерн в комплекс
			}
			else
			{
				//протестировать шаблон по маске
				//сгенерировать шаблон из статистики оставшихся после предыдущей выборки
				//удостовериться в наличии искомом паттерне в оставшейся выборке

				samples.calcGroupStat(maskCache[i - 1], false); //обновляем статистику по именам
				//ищем слово в новой стате
				auto word_pointer = samples.last_statistic_by_word[classter].find(
						globalStat[combination[i]].first);
				if (word_pointer != samples.last_statistic_by_word[classter].end())
				{
					//таки найдено!
					CTextPattern tp;
					tp.add(word_pointer->first->value);
					samples.testPattern(tp, maskCache[i - 1], false, true); //для всех паттернов,
					//для которых маска - false повторный поиск при успехе проставляет true
					maskCache[i] = samples.last_accepted_mask; //маску в кэш
					currentComplex.add(tp); //паттерн в комплекс
				}
				else
				{
					std::cout << "no word found => skipping" << std::endl;
					//пфф, генерируем новый
					//ан нет, не все так просто, надо скипнуть комбинации таким образом,
					//чтоб на хотя бы одном k-том, где k in [0;i] месте был другой элемент
					//при этом новых паттернов не добавляется.
					//вынесем в отдельный обработчик
					generate_new_combination = true;
				}
			}

			for(auto z: currentComplex.DNF)
			{
				for(auto zz: z[0].pattern)
				{
					std::cout<<zz.second.value<<std::endl;
				}
			}

			std::cout << "currentComplex.size() after =" << currentComplex.DNF.size() << std::endl;

			//проверяем покрытие
			int delta = samples.last_patter_statistic[classter].first
					- samples.last_patter_statistic[classter].second;

			std::cout << "delta: " << delta << " " << samples.last_patter_statistic[classter].first
					<< " of " << samples.last_patter_statistic[classter].second << std::endl;
			if (delta == 0 && !generate_new_combination)
			{
				std::cout << "cover is reached" << std::endl;
				//покрытие достигнуто
				//сравнить с лучшим, для этого посчитать энтропию
				std::vector<double> probability_data;
				for (auto x : samples.last_patter_statistic)
				{
					probability_data.push_back((double) x.second.first / (double) x.second.second);
				}
				double current_entropy = entropy(probability_data);
				if (current_entropy < best_entropy)
				{				//новый кандидат обнаружен
					bestComplex = currentComplex;
					best_entropy = current_entropy;
				}
				generate_new_combination = true;
			}
			//если достигнута максимальная длинна, также генерируем новый, даже если покрытие не достигнуто
			if (i == maxlen - 1)
			{
				generate_new_combination = true;
			}

			if (generate_new_combination)
			{
				generate_new_combination = false;
				//-------------------------------------
				bool is_valid = false;
				uint sameCount = 0;

				while (!is_valid)
				{
					prev_comb = combination;
					is_new_combination = nextCombination(combination, max_word_to_consider);
					if (!is_new_combination) //если больше ничего не сгенерировалось
						return bestComplex;
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
				for (auto d : combination)
					std::cout << " " << d;
				std::cout << std::endl;
				//тут найден валидный
				while (currentComplex.DNF.size() > sameCount)
					currentComplex.del();
				//идекс уже проверенности апгрейдить.
				already_estimated = sameCount;
				break;
				//-------------------------------------
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
