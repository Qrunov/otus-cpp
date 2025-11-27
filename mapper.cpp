#include <iostream>
#include <string>


using namespace std;

string::size_type	find_Nth(const string &s,int N,char ch, string::size_type pos = 0)
{
    while(N)
    {
	pos = s.find(ch, pos + 1);
	if (string::npos == pos)
	    return string::npos;
	N--;
    }
    return pos;
}

int main()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
	auto p1 = find_Nth(line, 9, ',');
	auto p2 = find_Nth(line, 1, ',', p1);
	string val = line.substr(p1 + 1,p2 - p1 - 1);
        std::cout << val << std::endl;
    }

    return 0;
}