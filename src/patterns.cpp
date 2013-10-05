//============================================================================
// Name        : patterns.cpp
// Author      : generall
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include "classes/pattern/CTokienPattern.h"
using namespace std;

int main()
{

	cout << levenshtein_distance(string("ololo"), string("olkoslo")) << endl; // prints
	return 0;
}
