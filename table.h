#pragma once
#include <map>
#include <tuple>
#include <vector>
#include "interfaces.h"

using content = std::vector<std::pair<int, std::string>>;

class Table
{
public:
    void truncate();
    bool insert(int id, const std::string &v);
    opResult intersect(const Table &t) const;
    opResult symmetric_difference(const Table &t) const;

    content select() const;

private:
    std::map<int, std::string> m_table;
};