#include "parser.h"

using namespace std;

bool cmdParser::parseIt(shared_ptr<sourceInterface> from, bool endPresumption)
{
	while (!from->wasFinished())
	{
		string cmd = from->getData();
		if (cmd == "")
			continue;
		else if (cmd == "{")
		{
			if (counter && !dynamicBlockCounter)
			{
				m_collector->endBlock();
				counter = 0;
			}
			dynamicBlockCounter++;
			if (dynamicBlockCounter == 1)
				m_collector->beginBlock();

			continue;
		}
		else if (cmd == "}")
		{
			dynamicBlockCounter--;
			counter = 0;
			if (!dynamicBlockCounter)
				m_collector->endBlock();
			else if (dynamicBlockCounter < 0)
				cout << "parse error! dynamic block unexpected end. Ignore it" << endl;
			continue;
		}

		if (!counter && !dynamicBlockCounter)
			m_collector->beginBlock();

		m_collector->addCmd(cmd);
		counter++;

		if (counter == m_N && !dynamicBlockCounter)
		{
			m_collector->endBlock();
			counter = 0;
		}
		// cout << cmd << endl;
	}
	if (endPresumption && counter && !dynamicBlockCounter)
		m_collector->endBlock();

	if (endPresumption)
	{
		counter = 0;
		dynamicBlockCounter = 0;
	}

	return true;
}
