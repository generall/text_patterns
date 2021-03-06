/*
 * libmathing.h
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#ifndef LIBMATHING_H_
#define LIBMATHING_H_

#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

typedef unsigned int uint;

namespace patterns
{

template<class T>
uint levenshtein_distance(const T & src, const T & dst)
{
	if (src == dst)
		return 0;

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

	std::vector<std::vector<uint> > l_matrix(
			(std::vector<std::vector<uint> >::size_type) (m + 1),
			std::vector<uint>());

	for (uint i = 0; i <= m; ++i)
	{
		l_matrix[i].resize(n + 1);
		l_matrix[i][0] = i;
	}
	for (uint i = 0; i <= n; ++i)
	{
		l_matrix[0][i] = i;
	}

	uint above_cell, left_cell, diagonal_cell, cost;

	for (uint i = 1; i <= m; ++i)
	{
		for (uint j = 1; j <= n; ++j)
		{
			cost = src[i - 1] == dst[j - 1] ? 0 : 1;
			above_cell = l_matrix[i - 1][j];
			left_cell = l_matrix[i][j - 1];
			diagonal_cell = l_matrix[i - 1][j - 1];
			l_matrix[i][j] = std::min(std::min(above_cell + 1, left_cell + 1),
					diagonal_cell + cost);
		}
	}

	return l_matrix[m][n];
}
std::wstring utf8to16(const std::string &s);
std::string utf16to8(const std::wstring &ws);

}

#endif /* LIBMATHING_H_ */
