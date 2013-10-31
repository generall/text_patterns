/*
 * metric.hpp
 *
 *  Created on: 30 окт. 2013 г.
 *      Author: generall
 */

#ifndef METRIC_HPP_
#define METRIC_HPP_

#include <vector>
#include <math.h>

namespace patterns
{

template<typename T>
T EuclideanDistance(const std::vector<T> &p1, const std::vector<T> &p2)
{
	T summ = 0;
	if (p1.size() != p2.size())
	{
		throw std::logic_error("Wrong vector Dimension");
	}
	for (int i = 0; i < p1.size(); i++)
	{
		summ += (p1[i] - p2[i]) * (p1[i] - p2[i]);
	}
	return sqrt(summ);

}

}
#endif /* METRIC_HPP_ */
