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
#include "CComplexAndSing.h"
#include "FPTree/FPTree.hpp"

namespace patterns
{

class CSamples
{

	bool debug = true;

	double entropy(std::vector<double> &data);
	bool nextCombination(std::vector<uint> & a, int n);

	uint max_word_to_consider = 300;

public:

	uint min_supply = 40;

	//глобальный массив примеров. Без структуры
	std::map<std::string, std::vector<CText *> > samples;

	std::map<std::string, std::vector<std::pair<CWord *, int> > > statistic;
	std::vector<std::pair<CWord *, int> > global_statistic;

	std::map<CWord *, int, CWordCompare> global_statistic_by_word;

	std::map<std::string, std::vector<bool> > last_accepted_mask;
	std::map<std::string, std::pair<uint, uint> > last_patter_statistic;

	//глобальный массив признаков.
	std::vector<TSignature *> signatures;
	//глобальная матрица Текст-признак по класстерам
	std::map<std::string, std::map<uint, std::map<uint, int> > > signature_matrix_by_text;
	std::map<std::string, std::map<uint, std::map<uint, int> > > signature_matrix_by_sign;

	//Признаки отсортированы по класстерам т.е. 1 - самый частый признак в кластере
	std::map<std::string, std::vector<TSignature *> > group_signatures;
	std::map<std::string, std::map<uint, std::map<uint, int> > > group_signature_matrix_by_text;
	std::map<std::string, std::map<uint, std::map<uint, int> > > group_signature_matrix_by_sign;
	std::map<std::string, std::map<uint, std::vector<uint> > > group_signature_matrix_by_text_sorted;

	std::map<std::string, FPTree<uint> > FPtree;

	std::map<std::string, std::vector<uint> > agregator;

	std::map<std::string, std::vector<double> > hyper_points;

	void init();


	//ALERT!
	//После выполнения этих процедер signature_matrix* становятся неактуальными и отображают неверные признаки относительно signature
	void createHyperspaceWordsOnly();
	void createHypeespaceWithComplex(bool with_words = false);
	void deleteInsignificantDimensions(double factor);
	void createTextHyperPoint(CText* text, std::vector<double> &hyper_point);


	uint groupToGlobal(uint index, const std::string &cluster);
	void groupToGlobal(std::vector<uint> &signs, const std::string &cluster);

	void loadFromFiles(std::string dir, std::string stoplist, bool has_puncluation = false,
			bool calcStatistics = false);

	void createMatrix();
	void createGroupMatrix();
	void createSortedMatrix();
	void createFPTree();
	void createAgregator();

	void FPFind(FPTree<uint> &tree, int delta_min, std::vector<uint> phi,
			std::vector<std::vector<uint> > &R);

	std::vector<std::vector<uint> > FPGrowth(const std::string &cluster, int delta_min);

	int getSignature(const std::string &cluster, uint text, uint sign);


	double testCover(const std::string &cluster, const std::vector<uint> &complex);
	double testCoverAnd(const std::string &cluster, const std::vector<uint> &complex);
	double testCoverAnd(const std::string &cluster, const std::vector<uint> &complex, std::vector<uint> &covered);

	std::vector<uint> getBestCover(std::vector<std::vector<uint> > &covers);
	std::vector<uint> getBestCoverAnd(std::vector<std::vector<uint> > &covers);

	void testPattern(const TPatternInterface &pattern);
	void testPattern(const TPatternInterface &pattern,
			std::map<std::string, std::vector<bool> > &mask, bool accept = false,
			bool renew = false);

	void calcGroupStat();

	void summStatistics(std::map<CWord*, int, CWordCompare> &s1,
			const std::map<CWord*, int, CWordCompare> &s2);

	std::vector<std::vector<uint> > generateCovers(const std::string &cluster, uint maxlen);

	CSamples();
	virtual ~CSamples();

	void correctErase();
};



class CPairComparator
{
public:
	bool operator()(std::pair<const uint, int>  &w1, std::pair<uint, int> &w2)
	{
		return (w1.first < w2.first);
	}
	bool operator()(std::pair<uint, int> & w1, std::pair<const uint, int> &w2)
	{
		return (w1.first < w2.first);
	}
};

} /* namespace patterns */
#endif /* CSAMPLES_H_ */
