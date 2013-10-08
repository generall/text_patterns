/*
 * CTextPattern.cpp
 *
 *  Created on: 07 окт. 2013 г.
 *      Author: generall
 */

#include "CTextPattern.h"

namespace patterns
{

CTextPattern::CTextPattern()
{

}

void CTextPattern::addBack(std::pair<CDelay, CTokenPattern>& p)
{
	pattern.push_back(p);
}

void CTextPattern::addFront(std::pair<CDelay, CTokenPattern>& p)
{
	pattern.push_front(p);
}

uint CTextPattern::compare(std::vector<CToken>& data)
{
	std::list<std::pair<CDelay, CTokenPattern> >::iterator it =
			pattern.begin();
	uint numOfSkippedTokens = 0;
	uint numOfFoundPatterns = 0;
	uint numAfterReset = 0;
	bool reset_pattern = false;

	std::vector<CToken>::iterator it_data = data.begin();

	//сложность в худшем случае - O(N*M*K) N - кол-во слов в тексте,
	// M - максимальное кол-во пропусков на токен K-максимальное кол-во токенов в шаблоне
	while (it_data != data.end())
	{
		numAfterReset++;
		CToken token = *it_data;

		if (it->second.compare(token))
		{
			it++;
			numOfSkippedTokens = 0;
			if (it == pattern.end())
			{
				/*
				 * Достигнут конец паттерна, следовательно, паттерн найден.
				 * Увеличить число найденных паттернов, продолжить поиск с начала паттерна.
				 * И с последнего места в тексте
				 */
				numOfFoundPatterns++;
				numAfterReset = 0;
				it = pattern.begin();
			}
		}
		else
		{
			/*
			 * Токен не соответсвует требуемому, рассмотреть возможность скипа.
			 */
			if (numOfSkippedTokens < it->first.maxDelayNumber)
			{
				if (it->first.isDelayAccaptable(token))
				{
					/*
					 * Пропускаем токен
					 */
					numOfSkippedTokens++;
				}
				else
				{
					/*
					 * Ресетаем паттерн и текст до паттерна
					 */
					reset_pattern = true;
				}
			}
			else
			{
				/*
				 * Превышен лимит пропусков
				 */
				reset_pattern = true;
			}
		}
		if (reset_pattern)
		{
			reset_pattern = false;
			it = pattern.begin();
			numOfSkippedTokens = 0;
			it_data -= numAfterReset - 2;
			numAfterReset = 0;
		}
		else
		{
			it_data++;
		}
	}

	return numOfFoundPatterns;
}

CTextPattern::~CTextPattern()
{

}

} /* namespace patterns */
