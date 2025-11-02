#include "parser.h"

using namespace std;

void cmdParser::parseIt(shared_ptr<ISource> from, bool endPresumption)
{
	while (!from->wasFinished())
	{
		string cmd = from->getData();
		parseByString(cmd);
	}
	if (endPresumption)
		parseEnd();
}

void cmdParser::parseByString(const std::string &cmd)
{
	if (cmd == "")
		return;
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

		return;
	}
	else if (cmd == "}")
	{
		dynamicBlockCounter--;
		counter = 0;
		if (!dynamicBlockCounter)
			m_collector->endBlock();
		else if (dynamicBlockCounter < 0)
			cout << "parse error! dynamic block unexpected end. Ignore it" << endl;
		return;
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
}

void cmdParser::parseEnd()
{
	if (counter && !dynamicBlockCounter)
		m_collector->endBlock();

	counter = 0;
	dynamicBlockCounter = 0;
}
