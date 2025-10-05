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

using fileMap = std::map<std::size_t, std::list<std::filesystem::path>>;
namespace fs = std::filesystem;

template <class di = fs::recursive_directory_iterator>
class searchFiles
{
public:
	fileMap doSearch(fs::path &dir)
	{
		fileMap res;
		auto d_it = di(dir);
		while (d_it != di())
		{
			auto &dir_entry = *d_it;
			decltype(exclude.begin()) it;
			if (dir_entry.is_directory() &&
				((it = std::find(exclude.begin(), exclude.end(), dir_entry.path())) != exclude.end() || (depth > -1 && d_it.depth() == depth)))
			{
				d_it.disable_recursion_pending(); // don't enter here
				d_it++;
				continue;
			}
			if (dir_entry.is_regular_file())
			{
				if (isMask)
				{
					if (!std::regex_match(dir_entry.path().filename().c_str(), mask))
						continue;
				}
				res[dir_entry.file_size()].push_back(dir_entry.path());
			}
			d_it++;
		}
		return res;
	}
	void setMask(std::regex m)
	{
		mask = m;
		isMask = true;
	}
	void setExcludeDir(std::vector<std::filesystem::path> &e)
	{
		exclude = e;
	}
	void setDepth(std::size_t d)
	{
		depth = d;
	}

private:
	std::regex mask;
	bool isMask{false};
	int depth{-1};
	std::vector<std::filesystem::path> exclude;
};
