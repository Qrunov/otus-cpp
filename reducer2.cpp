#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include "disp.h"

using namespace std;

int main()
{
    size_t count = 0;
    std::string line;
    vector<uint32_t>	v;
    while (std::getline(std::cin, line))
    {
        v.push_back(atoi(line.c_str()));
	count++;
    }
    cout << disp(v) << endl;

    return 0;
}