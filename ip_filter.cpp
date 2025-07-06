#include "ip_filter.h"
/*
ip::ip(const byte &f1, const byte &f2, const byte &f3, const byte &f4)
{
	byte *b = reinterpret_cast<byte *>(&n);
	b[0] = f4;
	b[1] = f3;
	b[2] = f2;
	b[3] = f1;
}
*/

void ip::set_field(int i, u_char f)
{
	u_char *b = reinterpret_cast<u_char *>(&n);
	b[sizeof(n) - i] = f;
}

bool ip::operator<=(const ip &c) const
{
	return n <= c.n;
	/*		for (decltype(n.size()) i = 0; i < n.size(); i++)
			{
				if (n[i] < c.n[i])
					return true;
				else if (n[i] > c.n[i])
					return false;

			}
			return true;*/
}
string ip::get_string() const
{
	auto *b = reinterpret_cast<const unsigned char *>(&n);
	stringstream ss;
	for (int i = sizeof(n) - 1; i >= 0;i-- )
	{
		ss << static_cast<uint>(b[i]); 
		if (i)
			ss << ".";
	}
	auto str = ss.str();
	return str;
}
u_char ip::get_field(int i) const
{
	const u_char *b = reinterpret_cast<const u_char *>(&n);
	return b[sizeof(n) - i];
}


tuple<ipParser::Error_code, vector<ip>> ipParser::parseStream(istream &ssource)
{
	regex pat{"^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\t\\w{1,}\t\\w{1,}"};
	//regex pat{"^(\\d{1,3}).(\\d{1,3}).(\\d{1,3}).(\\d{1,3})\t\\w{1,}\t\\w{1,}"};
    vector<ip> ips;
	Error_code result = Error_code::no_error;
	for (string line; getline(ssource, line);)
	{
		smatch matches;

		if (regex_search(line, matches, pat))
		{
			// cout << matches[1] << " " << matches[2] << " " << matches[3] << " " << matches[4] << '\n';
			auto convert = [&matches, &result](int i)
			{
				uint field = stoul(matches.str(i));
				if (field > static_cast<uint>(numeric_limits<u_char>::max()))
					result = Error_code::range_error;
				return static_cast<u_char>(field);
			};
			ip last_ip;
			for (decltype(matches.size()) i = 1; i < matches.size();i++)
			{
				last_ip.set_field(i, convert(i));
				if (Error_code::no_error != result)
					break;
			}
			if (Error_code::no_error != result)
				break;
           ips.push_back(last_ip);
        }
		else
		{
			result = Error_code::input_error;
			break;
		}
	}
	return make_tuple(result, ips);
}
