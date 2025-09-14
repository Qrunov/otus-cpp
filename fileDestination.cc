#include "fileDestination.h"
#include <fstream>
#include <iostream>

using namespace std;
void fileDestination::update(const block &b)
{
	string filename = to_string(b.time) + ".log";
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
