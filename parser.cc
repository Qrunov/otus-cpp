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
	unique_lock lock(m_lock, defer_lock);
        if (m_threadSave)
	    lock.lock();

	if (cmd == "")
	    return;

	if (m_next && cmd[0] != '{' && !m_dynamicBlockCounter)
	{
	    m_next -> parseByString(cmd);
	    return;
	}

	else if (cmd == "{")
	{
	    if (m_counter && !m_dynamicBlockCounter)
	    {
		m_collector->endBlock();
		m_counter = 0;
	    }
	    m_dynamicBlockCounter++;
	    if (m_dynamicBlockCounter == 1)
		m_collector->beginBlock();

	    return;
	}
	else if (cmd == "}")
	{
	    m_dynamicBlockCounter--;
	    m_counter = 0;
	    if (!m_dynamicBlockCounter)
		m_collector->endBlock();
	    else if (m_dynamicBlockCounter < 0)
		cout << "parse error! dynamic block unexpected end. Ignore it" << endl;
	    return;
	}

	if (!m_counter && !m_dynamicBlockCounter)
	    m_collector->beginBlock();

	m_collector->addCmd(cmd);
	m_counter++;

	if (m_counter == m_N && !m_dynamicBlockCounter)
	{
	    m_collector->endBlock();
	    m_counter = 0;
	}
}

void cmdParser::parseEnd()
{
	if (m_counter && !m_dynamicBlockCounter)
	    m_collector->endBlock();
	m_counter = 0;
	m_dynamicBlockCounter = 0;
}
