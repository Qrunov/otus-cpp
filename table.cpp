#include <table.h>

#include <iostream>

using namespace std;

void Table::truncate()
{
    m_table.clear();
}

bool Table::insert(int id, const std::string &v)
{
    if (m_table.find(id) != m_table.end())
        return false;
    m_table[id] = v;
    return true;
}

opResult Table::intersect(const Table &t) const
{
    opResult res;
    for (auto row1 : m_table)
    {
        auto it = t.m_table.find(row1.first);
        if (it != t.m_table.end())
            res.push_back(make_tuple(row1.first, row1.second, it->second));
    }
    return res;
}

opResult Table::symmetric_difference(const Table &t) const
{
    opResult res;
    for (auto row1 : m_table)
    {
        auto it = t.m_table.find(row1.first);
        if (it == t.m_table.end())
        {
        res.push_back(make_tuple(row1.first, row1.second, ""));

        }
            }

    for (auto row1 : t.m_table)
    {
        auto it = m_table.find(row1.first);
        if (it == m_table.end())
            res.push_back(make_tuple(row1.first, "", row1.second));
    }
    return res;
}

content Table::select() const
{
    content res;
    for (auto r : m_table)
        res.push_back(make_pair(r.first, r.second));
    return res;
}
