/*
 * CTFIDFSign.cpp
 *
 *  Created on: 01 дек. 2013 г.
 *      Author: generall
 */

#include "CTFIDFSign.h"

namespace patterns
{

CTFIDFSign::CTFIDFSign()
{
	// TODO Auto-generated constructor stub

}

double CTFIDFSign::test(CText* text)
{
	double r = this->CWeightWordSign::test(text);

	/*
	std::cout << "Word: " << word->value << " count: " << r << " factor: " << factor << " text_sz:"
			<< text->text.size() << " res: " << r * factor / (double) text->text.size()
			<< std::endl;
	int t;
	std::cin >> t;
	*/

	//return r*factor;

	return r * factor / (double) text->text.size(); // TF * IDF
}

CTFIDFSign::CTFIDFSign(CWord* w) :
		CWeightWordSign(w)
{

}

CTFIDFSign::~CTFIDFSign()
{
	// TODO Auto-generated destructor stub
}

} /* namespace patterns */
