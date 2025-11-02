#include "async.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

using namespace std;
size_t N = 3;

int main(int argc, const char *argv[])
{
    try
    {
        if (argc == 2)
            N = strtoul(argv[1], 0, 10);

        auto h = async::connect(N);
        while (!cin.eof())
        {
            string oneLine;
            cin >> oneLine;
            async::receive(h, oneLine.c_str(), oneLine.length());
        }

        async::disconnect(h);
    }
    catch (const std::exception &e)
    {
        cerr << "Runtime error has occured:" << e.what() << " program has been terminated" << endl;
    }
    return 0;
}
