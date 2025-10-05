#include <boost/program_options.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <iostream>
#include <string>
#include <list>
#include <cstdint>
#include <filesystem>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cassert>
#include <atomic>
#include <memory>
#include <regex>
#include "hasher.h"

namespace opt = boost::program_options;
namespace fs = std::filesystem;
namespace ipc = boost::interprocess;
using namespace std;

#include "fileRepresentation.h"
#include "searchFiles.h"
#include "filterDirPath.h"

class regexBuilder
{
public:
	static regex build(vector<string> masks)
	{
		string res;
		for (auto e : masks)
			res += (res.empty() ? string("") : string("|")) + "(" + regexBuilder::patternToRegex(e) + ")";

		cout << res << endl;
		return regex(res);
	}

private:
	static string patternToRegex(string &mask)
	{
		string regex_pattern;
		for (char c : mask)
		{
			switch (c)
			{
			case '*':
				regex_pattern += ".*";
				break; // * -> любое количество любых символов
			case '?':
				regex_pattern += ".";
				break; // ? -> один любой символ
			case '.':
				regex_pattern += "\\.";
				break; // экранируем точку
			case '[':
				regex_pattern += "[";
				break; // группы символов
			case ']':
				regex_pattern += "]";
				break;
			default:
				regex_pattern += c;
				break;
			}
		}
		return regex_pattern;
	}
};

void print_same(list<shared_ptr<fileRepresentation<>>> &may_same)
{
	map<size_t, list<shared_ptr<fileRepresentation<>>>> prePrint;
	size_t currentIndex = 0;
	for (auto it1 = may_same.begin(); it1 != may_same.end(); it1++)
	{
		auto it2 = it1;
		it2++;
		while (it2 != may_same.end())
		{
			auto it_rem = may_same.end();
			if (**it1 == **it2)
			{
				if (prePrint[currentIndex].empty())
					prePrint[currentIndex].push_back(*it1);
				prePrint[currentIndex].push_back(*it2);
				it_rem = it2;
			}
			it2++;
			if (it_rem != may_same.end())
				may_same.erase(it_rem);
		}
		if (!prePrint[currentIndex].empty())
			currentIndex++;
	}
	for (size_t i = 0; i < currentIndex; i++)
	{
		for (auto e : prePrint[i])
			cout << e->fullPath() << endl;
		cout << endl;
	}
};

void validateHashAlgValue(const string &value)
{
	auto l = hasherFactory::getInstance()->algorithmList();
	if (find(l.begin(), l.end(), value) == l.end())
		throw opt::validation_error(opt::validation_error::invalid_option_value, "alg", value);
}

int main(int argc, char *argv[])
{
	string hashAlg;
	auto l = hasherFactory::getInstance()->algorithmList();
	string algs;
	for (auto e : l)
		algs += algs.empty() ? e : string(", ") + e;

	opt::options_description desc("All options");
	desc.add_options()("include-path,I", opt::value<vector<string>>()->required()->multitoken(), "directory search list")("exclude-path,E", opt::value<vector<string>>()->multitoken(), "directory search black list(don't search there)")("level,L", opt::value<uint32_t>(), "restrict deep of directory search(0 - don't search in subdir's)")("filenames,F", opt::value<vector<string>>()->multitoken(), "file masks available for comparision")("bsize,S", opt::value<uint32_t>()->default_value(4096), "read block size(byte)")("alg", opt::value<string>(&hashAlg)->default_value(hasherFactory::getInstance()->getDefault())->notifier(&validateHashAlgValue), string("hash algorithms(" + algs + ")").c_str())("help,H", "produce help message");

	opt::variables_map vm;

	try
	{
		opt::store(opt::parse_command_line(argc, argv, desc), vm);
		opt::notify(vm);
	}
	catch (const opt::required_option &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		std::cout << desc << std::endl;
		return 1;
	}

	if (vm.count("help"))
	{
		cout << desc << endl;
		return 1;
	}

	searchFiles finder;

	if (vm.count("filenames"))
	{
		regex r = regexBuilder::build(vm["filenames"].as<vector<string>>());
		finder.setMask(r);
	}

	auto &v = vm["include-path"].as<vector<string>>();
	auto dirs = filterDirPath::filter(v);

	vector<string> exclude;
	vector<fs::path> dirsExclude;

	if (vm.count("exclude-path"))
	{
		exclude = vm["exclude-path"].as<vector<string>>();
		dirsExclude = filterDirPath::filter(exclude);
		finder.setExcludeDir(dirsExclude);
	}

	if (vm.count("level"))
		finder.setDepth(vm["level"].as<uint32_t>());

	decltype(finder.doSearch(dirs[0])) result;
	for (auto p : dirs)
	{
		auto files = finder.doSearch(p);
		for (auto e : files)
			result[e.first].splice(result[e.first].end(), e.second);
	}

	uint64_t blockSize = vm["bsize"].as<uint32_t>();
	auto h = hasherFactory::getInstance()->create(hashAlg);

	for (auto e : result)
	{
		list<shared_ptr<fileRepresentation<>>> may_same;
		for (auto it : e.second)
			may_same.push_back(make_shared<fileRepresentation<>>(it, e.first, blockSize, h));
		print_same(may_same);
	}
	return 0;
}
