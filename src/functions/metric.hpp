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

class TClassifierInterface
{
public:
	virtual double compare(const std::vector<double> &sample,
			const std::vector<double> &example) = 0;
	virtual ~TClassifierInterface()
	{
	}
};

class CEuclideanDistance: public TClassifierInterface
{
public:
	virtual double compare(const std::vector<double> &sample, const std::vector<double> &example)
	{
		double summ = 0;
		if (sample.size() != example.size())
		{
			throw std::logic_error("Wrong vector Dimension");
		}
		for (uint i = 0; i < sample.size(); i++)
		{
			summ += (sample[i] - example[i]) * (sample[i] - example[i]);
		}
		return sqrt(summ);
	}
};

class CNaiveBayes: public TClassifierInterface
{
public:
	double compare(const std::vector<double> &sample, const std::vector<double> &example)
	{
		double log_summ = 0;

		if (sample.size() != example.size())
		{
			throw std::logic_error("Wrong vector Dimension");
		}

		for (uint i = 0; i < example.size(); i++)
		{
			if (example[i] > 0.5)
			{ //i.e. == 1, in this context
			  //считаем вероятность появление 1
				if (sample[i] < std::numeric_limits<double>::epsilon())
				{
					log_summ += -log(1E-7);
				}
				else
				{
					log_summ += -log(sample[i]);
				}
			}
			else
			{
				double value = 1.0 - sample[i];	//вероятность того, что признака нет = 1-P, где P - вероятность того, что признак есть
				if (value < std::numeric_limits<double>::epsilon())
				{
					log_summ += -log(1E-7);
				}
				else
				{
					log_summ += -log(value);
				}
			}
		}
		return log_summ;
	}
};

}
#endif /* METRIC_HPP_ */
