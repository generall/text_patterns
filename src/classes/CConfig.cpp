/*
 * CConfig.cpp
 *
 *  Created on: 19 нояб. 2013 г.
 *      Author: generall
 */

#include "CConfig.h"

namespace patterns
{

CConfig::CConfig()
{
	// TODO Auto-generated constructor stub

}

std::string CConfig::getValue(const std::string& param)
{
	auto iter = config.find(param);
	if (iter == config.end())
	{
		return "";
	}
	return iter->second;
}

bool CConfig::loadConfig(const std::string& fname)
{

	std::fstream in(fname.c_str(), std::ios::in);
	if (in)
	{
		static char szDelims[] = " \t\n\r'";
		while (!in.eof())
		{
			std::string line, param, value;
			std::getline(in, line);
			if (line[0] != '#') //comment
			{
				line.erase(0, line.find_first_not_of(szDelims));
				line.erase(line.find_last_not_of(szDelims) + 1, param.size());
				if (line.size() > 1)
				{
					if (line.find('=') != std::string::npos)
					{

						std::stringstream text_strteam(line);
						std::getline(text_strteam, param, '=');

						//trim
						param.erase(0, param.find_first_not_of(szDelims));
						param.erase(param.find_last_not_of(szDelims) + 1, param.size());

						std::getline(text_strteam, value);
						value.erase(0, value.find_first_not_of(szDelims));
						value.erase(value.find_last_not_of(szDelims) + 1, param.size());
						config[param] = value;
					}
					else
					{
						throw std::logic_error("Wrong config format: " + line);
					}
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}

}

std::vector<std::string> CConfig::getClassVector()
{
	std::vector<std::string> classes;
	for (auto rec : config)
	{
		if (rec.first != "STOP_LIST" && rec.first != "TO_CLASSIFY" && rec.first != "USE_XML")
		{
			classes.push_back(rec.first);
		}
	}
	return classes;
}

CConfig::~CConfig()
{
	// TODO Auto-generated destructor stub
}

} /* namespace patterns */
