/*
 * CPunctuation.cpp
 *
 *  Created on: 05.10.2013
 *      Author: generall
 */

#include "CPunctuation.h"
namespace patterns
{


CPunctuation::CPunctuation(): punctuationType(p_default)
{
	setType(type_punctuation);
}

CPunctuation::CPunctuation(PunctuationType p) : punctuationType(p)
{
	setType(type_punctuation);
}

CPunctuation::~CPunctuation()
{


}

}
