#include "source.h"
#include <iostream>
using namespace std;

string grubFromCin::getData()
{
    string str;
    cin >> str;
    sleep(1);
    return str;
}
bool grubFromCin::wasFinished()
{
    return cin.eof();
}
