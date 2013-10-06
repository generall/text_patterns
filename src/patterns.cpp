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
	std::regex r("ol.*");
	cout << patterns::levenshtein_distance(string("ololo"), string("olkoslo")) << endl;
	cout << std::regex_match("ololo",r) << endl;
	cout << std::regex_match("trololo",r) << endl;
	return 0;
}
