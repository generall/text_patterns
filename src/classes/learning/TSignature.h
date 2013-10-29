/*
 * TSignature.h
 *
 *  Created on: 20 окт. 2013 г.
 *      Author: generall
 */

#ifndef TSIGNATURE_H_
#define TSIGNATURE_H_

#include "../text_representation/CText.h"

namespace patterns
{

class TSignature
{
public:
	uint place = 0; //место в глобальной статистике признаков
	virtual void print() = 0;
	virtual uint test(CText *) = 0;
	TSignature();
	virtual ~TSignature();
};

} /* namespace patterns */

#endif /* TSIGNATURE_H_ */
