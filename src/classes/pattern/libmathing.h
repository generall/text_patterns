/*
 * libmathing.h
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#ifndef LIBMATHING_H_
#define LIBMATHING_H_

#include <vector>
#include <algorithm>

typedef unsigned int uint;

namespace patterns
{

template<class T>
uint levenshtein_distance(const T & src, const T & dst)
{
	const uint m = src.size();
	const uint n = dst.size();
	if (m == 0)
	{
		return n;
	}
	if (n == 0)
	{
		return m;
	}

	std::vector<std::vector<uint> > matrix(m + 1);

	for (uint i = 0; i <= m; ++i)
	{
		matrix[i].resize(n + 1);
		matrix[i][0] = i;
	}
	for (uint i = 0; i <= n; ++i)
	{
		matrix[0][i] = i;
	}

	uint above_cell, left_cell, diagonal_cell, cost;

	for (uint i = 1; i <= m; ++i)
	{
		for (uint j = 1; j <= n; ++j)
		{
			cost = src[i - 1] == dst[j - 1] ? 0 : 1;
			above_cell = matrix[i - 1][j];
			left_cell = matrix[i][j - 1];
			diagonal_cell = matrix[i - 1][j - 1];
			matrix[i][j] = std::min(std::min(above_cell + 1, left_cell + 1),
					diagonal_cell + cost);
		}
	}

	return matrix[m][n];
}

}

#endif /* LIBMATHING_H_ */
