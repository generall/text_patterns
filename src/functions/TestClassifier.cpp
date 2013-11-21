/*
 * TestClassifier.cpp
 *
 *  Created on: 21 нояб. 2013 г.
 *      Author: generall
 */

#include "TestClassifier.hpp"

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

void executeClassifier(CSamples &teaching_selection, CSamples &examples,
		TClassifierInterface *classifier, const std::string file_name)
{

	std::fstream out(file_name.c_str(), std::ios::out | std::ios::trunc);

	for (auto example_cluster : examples.samples)
	{
		for (auto text : example_cluster.second)
		{
			double min_dist = INFINITY;
			std::string nearest_cluster = "";
			std::vector<double> point;
			teaching_selection.createTextHyperPoint(text, point); // Создаем точку для текста по признакам, содержащится в signatures

			for (auto selection_hyperpoint : teaching_selection.hyper_points)
			{
				//инициализация классификатора дисперсией (существенна(выполняется) только для Махланобиса)
				classifier->init_dispersion(
						teaching_selection.hyper_points_dispersion[selection_hyperpoint.first]);

				double dist = classifier->compare(selection_hyperpoint.second, point);
				if (dist < min_dist)
				{
					nearest_cluster = selection_hyperpoint.first;
					min_dist = dist;
				}
			}
			out << text->file_name << "=" << nearest_cluster << std::endl;
		}
	}
	out.close();
}

}
