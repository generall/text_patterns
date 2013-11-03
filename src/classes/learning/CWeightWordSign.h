/*
 * CWeightWordSign.h
 *
 *  Created on: 03 нояб. 2013 г.
 *      Author: generall
 */

#ifndef CWEIGHTWORDSIGN_H_
#define CWEIGHTWORDSIGN_H_

#include "TSignature.h"
#include "CWordSign.h"

namespace patterns
{

class CWeightWordSign: public patterns::CWordSign
{
public:
	virtual uint test(CText *);
	CWeightWordSign();
	CWeightWordSign(CWord * word);

	virtual ~CWeightWordSign();
};

} /* namespace patterns */

#endif /* CWEIGHTWORDSIGN_H_ */
