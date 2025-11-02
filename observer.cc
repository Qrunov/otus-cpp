#include "interfaces.h"
#include <list>
#include <memory>

using namespace std;
void subject::registration(const shared_ptr<observer>& ob)
{
    m_observer_list.push_back(ob);
}

void subject::notify(const commandBlock &b) const
{
    for (auto e : m_observer_list)
        e->update(b);
}
