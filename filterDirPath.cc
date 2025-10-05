#include "filterDirPath.h"

namespace fs = std::filesystem;
using namespace std;

vector<fs::path> filterDirPath::filter(vector<string> in)
{
	vector<fs::path> res;
	for (auto it = in.begin(); it != in.end(); it++)
	{
		error_code ec;
		fs::path d1 = fs::canonical(*it, ec);
		if (ec)
		{
			cerr << "error path " << *it << ": " << ec.message() << endl;
			continue;
		}
		if (!fs::is_directory(d1))
		{
			cerr << "path " << *it << " isn't directory" << endl;
			continue;
		}
		d1 = fs::absolute(d1);
		d1 = d1.lexically_normal();
		res.push_back(d1);
	}
	sort(res.begin(), res.end());
	for (auto itUp = res.begin(); itUp != res.end(); itUp++)
	{
		auto itDown = itUp;
		itDown++;
		while (itDown != res.end())
		{
			if (isDirRelative(*itUp, *itDown))
				itDown = res.erase(itDown);
			else
				itDown++;
		}
	}
	return res;
}

bool filterDirPath::isDirRelative(fs::path dir1, fs::path dir2)
{
	auto it1 = dir1.begin();
	auto it2 = dir2.begin();
	for (; it1 != dir1.end() && it2 != dir2.end(); it1++, it2++)
		if (*it1 != *it2)
			return false;
	return it1 == dir1.end();
}
