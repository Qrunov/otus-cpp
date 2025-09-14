#include "interfaces.h"
#include <list>
#include <memory>

using namespace std;
void subject::registration(shared_ptr<observer> ob)
{
    m_observer_list.push_back(ob);
}

void subject::notify(const block &b)
{
    for (auto e : m_observer_list)
        e->update(b);
}
