#include <iostream>
#include <mutex>
#include "controller.h"

using namespace std;

shared_ptr<Controller> Controller::m_instance;

Controller::Controller()
{
    m_tables["A"] = make_unique<Table>();
    m_tables["B"] = make_unique<Table>();
}

std::shared_ptr<Controller> Controller::getInstance()
{
    if (!m_instance)
        m_instance.reset(new Controller());
    return m_instance;
}

std::string Controller::truncate(string table)
{
    std::unique_lock lock(m_globalLock);
    if (m_tables.find(table) != m_tables.end())
    {
        m_tables[table]->truncate();
        return string("< OK");
    }
    else
        return string("< ERR no such table ") + table;
}

std::string Controller::insert(string table, int id, string value)
{
    std::unique_lock lock(m_globalLock);
    if (m_tables.find(table) != m_tables.end())
    {
        if (m_tables[table]->insert(id, value))
            return string("< OK");
        else
            return string("< ERR duplicate ") + to_string(id);
    }
    else
        return string("< ERR no such table ") + table;
}

pair<opResult, std::string> Controller::intersect(const string &table1, const string &table2)
{
    std::shared_lock lock(m_globalLock);
    opResult res;
    if (m_tables.find(table1) == m_tables.end())
        return make_pair(res, string("< ERR no such table ") + table1);

    if (m_tables.find(table2) == m_tables.end())
        return make_pair(res, string("< ERR no such table ") + table2);

    res = m_tables[table1]->intersect(*m_tables[table2]);
    return make_pair(res, "< OK");
}

pair<opResult, std::string> Controller::symmetric_difference(const std::string &table1, const std::string &table2)
{
    std::shared_lock lock(m_globalLock);
    opResult res;
    if (m_tables.find(table1) == m_tables.end())
        return make_pair(res, string("< ERR no such table ") + table1);

    if (m_tables.find(table2) == m_tables.end())
        return make_pair(res, string("< ERR no such table ") + table2);

    res = m_tables[table1]->symmetric_difference(*m_tables[table2]);
    return make_pair(res, "< OK");
}
