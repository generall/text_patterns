/*
 * CKnearest.cpp
 *
 *  Created on: 02 дек. 2013 г.
 *      Author: generall
 */

#include "CKnearest.h"

namespace patterns
{

CKnearest::CKnearest(CSamples* s)
{
	samples = s;
}

void CKnearest::create_points()
{
	uint hyperspace_size = samples->signatures.size();

	for (auto cluster : samples->samples)
	{
		clusters.push_back(cluster.first);

		for (auto text : samples->signature_matrix_by_text[cluster.first])
		{
			std::vector<double> point;
			point.resize(hyperspace_size, 0);

			for (auto sign : text.second)
			{
				//point[sign.first] = 1;
				//TFIDF

				double TF = (double) sign.second
						/ (double) samples->samples[cluster.first][text.first]->text.size();
				point[sign.first] = TF * samples->IDF[sign.first];

			}
			points.push_back(std::make_pair(clusters.size() - 1, point));

			std::cout << "created point cluster:" << clusters.size() - 1 << std::endl;
		}
	}
	std::cout << "points created. size:" << points.size() << std::endl;
}

std::string CKnearest::classify(std::vector<double> example, TClassifierInterface &metric)
{
	std::vector<std::pair<uint, double> > nearest;
	std::vector<int> clusters_count;

	clusters_count.resize(clusters.size(), 0);

	nearest.resize(k + 1, std::make_pair(0, INFINITY));

	static int count_of_call = 0;
	std::cout << count_of_call++ << " -- call" << std::endl;

	/*
	int t = 0;
	std::cin >> t;
	*/

	for (auto point : points)
	{
		double dist = metric.compare(point.second, example);

		//std::cout << "point class: " << point.first << " dist: " << dist << std::endl;
		nearest[k].first = point.first;
		nearest[k].second = dist;

		for (int i = k - 1; i > 0; i--)
		{
			if (nearest[i + 1].second < nearest[i].second)
			{

				//std::cout << "swapped class: " << nearest[i + 1].first << std::endl;
				std::swap(nearest[i + 1], nearest[i]);
			}
			else
			{
				break;
			}
		}
	}

	int maxi = 0;
	for (auto near_point : nearest)
	{
		clusters_count[near_point.first]++;
		if (clusters_count[maxi] < clusters_count[near_point.first])
		{
			maxi = near_point.first;
		}
	}
	std::cout << "cluster: " << maxi << std::endl;
	return clusters[maxi];
}

CKnearest::~CKnearest()
{
	// TODO Auto-generated destructor stub
}

} /* namespace patterns */
