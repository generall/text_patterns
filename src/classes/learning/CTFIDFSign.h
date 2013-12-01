/*
 * CTFIDFSign.h
 *
 *  Created on: 01 дек. 2013 г.
 *      Author: generall
 */

#ifndef CTFIDFSIGN_H_
#define CTFIDFSIGN_H_

#include "CWeightWordSign.h"

namespace patterns
{

class CTFIDFSign: public patterns::CWeightWordSign
{
public:
	double factor = 1;

	virtual double test(CText* text);
	CTFIDFSign();
	CTFIDFSign(CWord *);
	virtual ~CTFIDFSign();
};

} /* namespace patterns */

#endif /* CTFIDFSIGN_H_ */
