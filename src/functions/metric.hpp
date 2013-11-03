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
	void normalizeVector(std::vector<double> &vector)
	{
		//для векторов, полученных из единичного документа.
		double length = 0;
		for (uint i = 0; i < vector.size(); i++)
		{
			length += vector[i] * vector[i];
		}
		length = sqrt(length);
		for (uint i = 0; i < vector.size(); i++)
		{
			vector[i] = vector[i] / length;
		}
	}

	void binarization(std::vector<double> &vector)
	{
		for (uint i = 0; i < vector.size(); i++)
		{
			vector[i] = vector[i] > 0.5 ? 1 : 0;
		}
	}

	virtual double compare(const std::vector<double> &sample, const std::vector<double> &example)
	{
		//normalize vector before.
		auto n_sample = sample;
		auto n_example = example;
		normalizeVector(n_sample);
		normalizeVector(n_example);

		double summ = 0;
		if (n_sample.size() != n_example.size())
		{
			throw std::logic_error("Wrong vector Dimension");
		}
		for (uint i = 0; i < n_sample.size(); i++)
		{
			summ += (n_sample[i] - n_example[i]) * (n_sample[i] - n_example[i]);
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
