#include "source.h"
#include <iostream>
#include <unistd.h>


using namespace std;

string grubFromCin::getData()
{
    string ret;
    cin >> ret;
    return ret;
}

bool grubFromCin::wasFinished()
{
    return cin.eof();
}

string grubFromString::getData()
{
    string ret;
    string_data >> ret;
    return ret;
}
bool   grubFromString::wasFinished()
{
    return string_data.eof();
}
