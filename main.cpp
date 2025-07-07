#include "ip_filter.h"

int main(int, char **)
{
	auto [r, ipvec] = ipParser::parseStream(cin);
	if (ipParser::Error_code::no_error != r)
	{
		cerr << "parse error has occured";
		return -1;
	}
	auto ip_cmp_gt = [](const ip &ip1, const ip &ip2)
	{
		return !(ip1 <= ip2);
	};
	sort(ipvec.begin(), ipvec.end(), ip_cmp_gt);

	auto output = [](const ip &out)
	{
		cout << out.get_string() << endl;
	};

	auto f1 = [](const ip &out)
	{
		if (1 == out.get_field(1))
			cout << out.get_string() << endl;
	};

	auto f2 = [](const ip &out)
	{
		if (46 == out.get_field(1) && 70 == out.get_field(2))
			cout << out.get_string() << endl;
	};

	auto f3 = [](const ip &out)
	{
		if (46 == out.get_field(1) || 46 == out.get_field(2) || 46 == out.get_field(3) || 46 == out.get_field(4))
			cout << out.get_string() << endl;
	};

	array<function<void(const ip &)>, 4> ls{output, f1, f2, f3};

	for (const auto &do_out : ls)
		for_each(ipvec.begin(), ipvec.end(), do_out);

	return 0;
}
