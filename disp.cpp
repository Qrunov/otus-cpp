#include "disp.h"
#include <iostream>

using namespace std;

double disp(const vector<uint32_t>& v)
{
    double mean = 0.0;
    for (auto e: v)
	mean += e;
    mean /= v.size();

    double res = 0.0;
    for (auto e: v)
	res += (mean - e) * (mean - e);


    return res / (v.size() - 1);
}
