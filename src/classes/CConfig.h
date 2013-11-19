/*
 * CConfig.h
 *
 *  Created on: 19 нояб. 2013 г.
 *      Author: generall
 */

#ifndef CCONFIG_H_
#define CCONFIG_H_

#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

namespace patterns
{

class CConfig
{

	std::map<std::string, std::string> config;

public:

	std::string getValue(const std::string &param);

	bool loadConfig(const std::string &fname);

	std::vector<std::string> getClassVector();

	CConfig();
	virtual ~CConfig();
};

} /* namespace patterns */

#endif /* CCONFIG_H_ */
