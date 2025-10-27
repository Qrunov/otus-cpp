#include "fileDestination.h"
#include <fstream>
#include <iostream>
#include <thread>

using namespace std;
void fileDestination::update(const block &b)
{
	if (b.time == last)
	    counter++;
	else
	{
	    counter = 0;
	    last = b.time;
	}
	string filename = to_string(pthread_self()) + "_" + to_string(b.time) + "_" + to_string(counter) + ".log";
//	string filename = to_string(b.time) + "_" + to_string(counter) + ".log";
	fstream fout(filename, ios_base::out);
	if (!fout.is_open())
	{
		cout << "failed to open " << filename << '\n';
		return;
	}

	for (auto e : b.cmd)
	{
		fout << e << endl;
	}
}
