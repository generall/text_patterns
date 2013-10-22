/*
 * FPNode.hpp
 *
 *  Created on: 22 окт. 2013 г.
 *      Author: generall
 */

#ifndef FPNODE_HPP_
#define FPNODE_HPP_

#include <vector>
#include <map>

namespace patterns
{

template<typename T>
class FPNode
{
public:

	FPNode<T> *parent;
	std::map<T, FPNode<T> *> offspring;


	FPNode<T>* getOffspring(T &v)
	{
		auto i = offspring.find(v);
		if(i == offspring.end())
			return NULL;
		return i->second;
	}


	int count = 1;
	T value;

	FPNode(T v, FPNode<T> *p)
	{
		value = v;
		parent = p;
	}


	virtual ~FPNode()
	{
	}

	bool operator==(T v)
	{
		return v == value;
	}

};

} /* namespace patterns */
#endif /* FPNODE_HPP_ */

