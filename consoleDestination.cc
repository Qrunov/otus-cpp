#include "interfaces.h"
#include "consoleDestination.h"
#include <iostream>

using namespace std;

void consoleDestination::update(const block &b)
{
	if (b.cmd.size())
		cout << "bulk: ";
	uint32_t i = 0;
	for (auto e : b.cmd)
	{
		i++;
		cout << e << (i < b.cmd.size() ? ", " : "");
	}
	cout << endl;
}
