/*
 * FPTree.hpp
 *
 *  Created on: 22 окт. 2013 г.
 *      Author: generall
 */

#ifndef FPTREE_HPP_
#define FPTREE_HPP_

#include "FPNode.hpp"
#include <iostream>

namespace patterns
{

template<typename T>
class FPTree
{
public:
	FPNode<T> *root;
	FPNode<T> *current;
	std::map<T, std::pair<std::vector<FPNode<T>*>, int> > pointers;

	FPTree()
	{
		root = new FPNode<T>(0, NULL);
		current = root;
	}

	void rDelete0(FPNode<T> * node)
	{
		for (auto x : node->offspring)
		{
			if (x.second->count == 0)
			{
				rDelete0(x.second);
				delete x.second;
				//WARNING! SEGFAULT EXPECTED
				node->offspring.erase(x.first);
			}else{
				rDelete0(x.second);
			}
		}
	}

	void rebuidPointers(FPNode<T> * node)
	{

		for (auto x : node->offspring)
		{
			pointers[x.first].second += x.second->count;
			pointers[x.first].first.push_back(x.second);
			rebuidPointers(x.second);
		}
	}
	void copy1(FPNode<T> * from, FPNode<T> * to)
	{
		for (auto x : from->offspring)
		{
			FPNode<T> *new_node = new FPNode<T>(x.first, to);
			to->offspring[x.first] = new_node;
			new_node->count = x.second->count;
			pointers[x.first].first.push_back(new_node);
			copy1(x.second, new_node);
		}
	}

	void dropCount(FPNode<T> * node, T v)
	{
		for (auto x : node->offspring)
		{
			if (x.first != v)
			{
				x.second->count = 0;
			}
			dropCount(x.second, v);
		}
	}

	void FPAdd(T v)
	{
		FPNode<T> * t = current->getOffspring(v);
		if (t != NULL)
		{
			t->count++;
			pointers[v].second += 1;
			current = t;
		}
		else
		{
			t = new FPNode<T>(v, current);
			if(pointers.find(v) == pointers.end())
			{
				pointers[v] = std::make_pair(std::vector<FPNode<T>*>(), 1);
				pointers[v].first.push_back(t);
			}else{
				pointers[v].second += 1;
				pointers[v].first.push_back(t);
			}
			current->offspring[v] = t;
			current = t;
		}
	}
	void switchToRoot()
	{
		current = root;
	}

	FPTree(FPTree &t, T v)
	{
		root = new FPNode<T>(0, NULL);
		current = root;
		copy1(t.root, root);
		dropCount(root, v);

		for (auto x : pointers[v].first)
		{
			FPNode<T> *temp = x;
			while (temp->parent != NULL)
			{
				temp->parent->count += temp->count;
				temp = temp->parent;
			}
			x->count = 0;
		}

		rDelete0(root);
		pointers.clear();
		rebuidPointers(root);
	}

	void print(FPNode<T> * node, int n)
	{
		if (n < 15)
		{
			for (auto x : node->offspring)
			{
				std::cout << std::string(n, '-') << " ";
				std::cout << x.first<< " : "<<x.second->count<<" :: "<<pointers[x.first].second << std::endl;
				print(x.second, n + 1);
			}
		}
	}

	virtual ~FPTree()
	{
		for (auto x : pointers)
		{
			for (auto y : x.second.first)
			{
				delete y;
			}
		}
	}
};

} /* namespace patterns */

#endif /* FPTREE_HPP_ */
