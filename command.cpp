#include <iostream>
#include "command.h"

using namespace std;

void Command::execute(std::string cmd)
{
	stringstream parse(cmd);
	string word;
	parse >> word;
	opResult join;
	if ("TRUNCATE" == word)
	{
		word = "";
		parse >> word;
		if (word.empty() || word == "\n")
		{
			m_result << "< ERR: NO TABLE NAME";
			return;
		}
		m_result << m_controller->truncate(word);
	}
	else if ("INSERT" == word)
	{
		int id;
		string w, table;
		word = "";
		parse >> word;
		if (word.empty() || word == "\n")
		{
			m_result << "< ERR: NO TABLE NAME";

			return;
		}
		table = word;
		word = "";
		parse >> word;
		if (word.empty() || word == "\n")
		{
			m_result << "< ERR: NO ID";
			return;
		}
		id = atoi(word.c_str());

		word = "";
		parse >> word;
		if (word.empty() || word == "\n")
		{
			m_result << "< ERR: NO VALUE";
			return;
		}
		w = word;

		m_result << m_controller->insert(table, id, w);
	}
	else if ("INTERSECTION" == word)
	{
		auto r = m_controller->intersect("A", "B");
		if ("< OK" == r.second)
			join = r.first;
		else
			m_result << r.second;
	}
	else if ("SYMMETRIC_DIFFERENCE" == word)
	{
		auto r = m_controller->symmetric_difference("A", "B");
		if ("< OK" == r.second)
			join = r.first;
		else
			m_result << r.second;
	}
	else
		m_result << "< ERR: NO SUCH COMMAND: " << word;

	if (join.size())
	{
		for (auto r : join)
			m_result << "< " << get<0>(r) << "," << get<1>(r) << "," << get<2>(r) << endl;
		m_result << "< OK";
	}
}
