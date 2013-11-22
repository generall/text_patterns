/*
 * CControl.cpp
 *
 *  Created on: 21 нояб. 2013 г.
 *      Author: generall
 */

#include "CControl.h"

namespace patterns
{

bool operator==(std::vector<uint> v1, std::vector<uint> v2)
{
	if (v1.size() != v2.size())
	{
		return false;
	}
	for (uint i = 0; i < v1.size(); i++)
	{
		if (v1[i] != v2[i])
			return false;
	}
	return true;
}

CControl::CControl()
{
	argc = 0;
	argv = NULL;
	do_classify = false;
// TODO Auto-generated constructor stub

}

void CControl::init(int _argc, char** _argv)
{
	argc = _argc;
	argv = _argv;

	enum State
	{
		s_none, s_input, s_output, s_error
	};

	State c_stat = s_none;

	for (int i = 1; i < argc; i++)
	{

		switch (argv[i][0])
		{
		case '-':
			if (c_stat != s_none)
			{
				c_stat = s_error;
				break;
			}
			switch (argv[i][1])
			{
			case 'k':
				do_classify = false;
				break;
			case 'c':
				c_stat = s_input;
				break;
			case 'o':
				c_stat = s_output;
				break;
			default:
				c_stat = s_error;
				break;
			}
			break;
		default:
			switch (c_stat)
			{
			case s_none:
				c_stat = s_error;
				break;
			case s_input:
				conf_filename = std::string(argv[i]);
				c_stat = s_none;
				break;
			case s_output:
				output_filename = std::string(argv[i]);
				c_stat = s_none;
				break;
			default:
				c_stat = s_error;
				break;
			}
			break;
		}

		if (c_stat == s_error)
		{
			throw std::logic_error("Wrong programm parameters");
		}
	}

	if (output_filename.size() == 0 || conf_filename.size() == 0)
	{
		throw std::logic_error("No filename " + std::to_string(argc));
	}

}

void CControl::classify(CConfig &config, CSamples &s, CSamples &test_texts)
{

	patterns::CAngle angle;
	s.createHyperspaceWordsOnly();
	/*
	 * Классификация
	 */

	executeClassifier(s, test_texts, &angle, output_filename);

}

int CControl::execute()
{
	/*
	 * Загрузка конфига
	 */
	patterns::CConfig config;
	config.loadConfig(conf_filename);
	bool use_xml = true;
	use_xml = config.getValue("USE_XML") != "NO";
	auto clusters = config.getClassVector();
	auto stoplist = config.getValue("STOP_LIST");

	CSamples s;
	/*
	 * Загрузка файлов
	 */
	for (auto x : clusters)
	{
		s.loadFromFiles(config.getValue(x), stoplist, x, false, true, use_xml);
	}
	s.init();

	if (do_classify)
	{
		CSamples test_texts;
		test_texts.loadFromFiles(config.getValue("TO_CLASSIFY"), stoplist, "test", false, true,
				use_xml);
		classify(config, s, test_texts);
	}
	else
	{
		keywords(config, s);
	}
	return 0;
}

void CControl::keywords(CConfig &config, CSamples &s)
{
	/*
	 * Загрузка конфига
	 */

	std::map<std::string, std::vector<std::vector<uint> > > f_words;

	std::vector<std::string> clusters;
	for (auto x : s.signature_matrix_by_sign)
	{
		clusters.push_back(x.first);
	}

	for (auto cluster : clusters)
	{
		int supply = ((double) s.samples[cluster].size())
				* ((double) std::stoi(config.getValue("MIN_SUPPLY")) / 100.0);

		f_words[cluster] = s.FPGrowth(cluster, supply, true);
		//debug
		std::cout << cluster << " " << f_words[cluster].size() << std::endl;
	}

	for (auto cluster : clusters)
	{
		std::vector<std::vector<uint> > converted;
		for (uint i = 0; i < f_words[cluster].size(); i++)
		{
			converted.push_back(std::vector<uint>());
			for (uint j = 0; j < f_words[cluster][i].size(); j++)
			{
				converted[i].push_back(s.groupToGlobal(f_words[cluster][i][j], cluster));
			}
		}
		f_words[cluster] = converted;

		//debug
		std::cout << cluster << " " << f_words[cluster].size() << std::endl;
	}

	std::string target = config.getValue("TARGET");

//уникализировать.
	for (auto cluster : clusters)
	{
		if (cluster == target)
		{
			continue;
		}
		for (auto complex : f_words[cluster])
		{

			for (uint i = 0; i < f_words[target].size(); i++)
			{
				if (f_words[target][i] == complex)
				{
					f_words[target].erase(f_words[target].begin() + i);
					i--; // EVIL EVIL EVIL
				}
			}
		}
	}

	std::cout << "start write " << f_words[target].size() << std::endl;
	std::fstream out(output_filename, std::ios::out | std::ios::trunc);
	for (auto complex : f_words[target])
	{
		for (auto int_word : complex)
		{
			out << s.global_statistic[int_word].first->value << " ";
		}
		out << std::endl;
	}
	out.close();
}

CControl::~CControl()
{
// TODO Auto-generated destructor stub
}

} /* namespace patterns */
