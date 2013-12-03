/*
 * TestClassifier.hpp
 *
 *  Created on: 30 окт. 2013 г.
 *      Author: generall
 */

#ifndef TESTCLASSIFIER_HPP_
#define TESTCLASSIFIER_HPP_

#include "../classes/learning/CSamples.h"
#include "metric.hpp"
#include "CKnearest.h"

namespace patterns
{

double testClassifier(CSamples &teaching_selection, CSamples &examples,
		TClassifierInterface *classifier)
{

	uint correct = 0;
	uint total = 0;
	for (auto example_cluster : examples.samples)
	{
		std::string target_clusler = example_cluster.first;
		for (auto text : example_cluster.second)
		{
			double min_dist = INFINITY;
			std::string nearest_cluster = "";
			std::vector<double> point;
			teaching_selection.createTextHyperPoint(text, point);

			for (auto selection_hyperpoint : teaching_selection.hyper_points)
			{
				//инициализация классификатора дисперсией (существенна только для Махланобиса)
				classifier->init_dispersion(
						teaching_selection.hyper_points_dispersion[selection_hyperpoint.first]);

				double dist = classifier->compare(selection_hyperpoint.second, point);
				if (dist < min_dist)
				{
					nearest_cluster = selection_hyperpoint.first;
					min_dist = dist;
				}
			}
			if (nearest_cluster == target_clusler)
			{
				correct++;
			}
			else
			{
				/*
				 std::cout << "Wrong: " << nearest_cluster << " actualy is " << target_clusler
				 << " min_dist: " << min_dist << std::endl;
				 */
			}
			total++;
			//std::cout << correct << " of " << total << std::endl;
		}
	}
	return (double) correct / (double) total;
}

double testKnearestClassifier(CSamples &teaching_selection, CSamples &examples,
		TClassifierInterface *classifier)
{

	CKnearest k_nearest(&teaching_selection);
	k_nearest.create_points();

	uint correct = 0;
	uint total = 0;
	for (auto example_cluster : examples.samples)
	{
		std::string target_clusler = example_cluster.first;
		for (auto text : example_cluster.second)
		{
			std::vector<double> point;
			teaching_selection.createTextHyperPoint(text, point);
			std::string nearest_cluster = k_nearest.classify(point, *classifier);
			if (nearest_cluster == target_clusler)
			{
				correct++;
			}
			total++;
		}
		std::cout << "progress: " << total << std::endl;
	}
	return (double) correct / (double) total;
}

}
#endif /* TESTCLASSIFIER_HPP_ */
