/*
 * CWordSign.h
 *
 *  Created on: 20 окт. 2013 г.
 *      Author: generall
 */

#ifndef CWORDSIGN_H_
#define CWORDSIGN_H_

#include "TSignature.h"

namespace patterns
{

class CWordSign: public virtual patterns::TSignature
{
public:
	CWord* word = NULL;
	virtual uint test(CText * text);
	virtual void print();
	CWordSign();
	CWordSign(CWord* _word);
	virtual ~CWordSign();
};

} /* namespace patterns */

#endif /* CWORDSIGN_H_ */
