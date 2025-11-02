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

bool grubFromCin::wasFinished() const
{
    return cin.eof();
}

string grubFromString::getData() 
{
    string ret;
    string_data >> ret;
    return ret;
}
bool   grubFromString::wasFinished() const
{
    return string_data.eof();
}
