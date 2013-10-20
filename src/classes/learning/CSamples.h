/*
 * CSamples.h
 *
 *  Created on: 15 окт. 2013 г.
 *      Author: generall
 */

#ifndef CSAMPLES_H_
#define CSAMPLES_H_

#include <vector>
#include <utility>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "../text_representation/CText.h"
#include "../pattern/TPatternInterface.h"
#include "TSignature.h"
#include "CWordSign.h"

namespace patterns
{

class CSamples
{

public:

	//глобальный массив примеров. Без структуры
	std::map<std::string, std::vector<CText *> > samples;

	std::map<std::string, std::vector<std::pair<CWord *, int> > > statistic;
	std::vector<std::pair<CWord *, int> > global_statistic;

	std::map<std::string, std::vector<std::pair<CWord *, int> > > last_statistic;
	std::map<std::string, std::map<CWord *, int, CWordCompare> > last_statistic_by_word;
	std::map<CWord *, int, CWordCompare> global_statistic_by_word;

	std::map<std::string, std::vector<bool> > last_accepted_mask;
	std::map<std::string, std::pair<uint, uint> > last_patter_statistic;

	//глобальный массив признаков.
	std::vector<TSignature *> signatures;
	//глобальная матрица Текст-признак по класстерам, наверно
	std::map<std::string, std::map<uint, std::map<uint, int> > > signature_matrix_by_text;
	std::map<std::string, std::map<uint, std::map<uint, int> > > signature_matrix_by_sign;

	void loadFromFiles(std::string dir, bool has_puncluation = false, bool calcStatistics = false);
	void createMatrix();
	int getSignature(const std::string &cluster, uint text, uint sign);

	void testPattern(const TPatternInterface &pattern);
	void testPattern(const TPatternInterface &pattern,
			std::map<std::string, std::vector<bool> > &mask, bool accept = false,
			bool renew = false);

	void calcGroupStat();
	void calcGroupStat(std::map<std::string, std::vector<bool> > &mask, bool accept = true);

	void summStatistics(std::map<CWord*, int, CWordCompare> &s1,
			const std::map<CWord*, int, CWordCompare> &s2);

	CSamples();
	virtual ~CSamples();
};

} /* namespace patterns */

#endif /* CSAMPLES_H_ */
