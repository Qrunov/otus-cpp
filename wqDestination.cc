#include "wqDestination.h"
#include <memory>
#include <fstream>
#include <iostream>

using namespace std;
void wqDestination::update(const block &b)
{
    if (!m_queue)
    {
	cerr << "no destination query exists" << endl;
	return;
    }

    auto b_copy = make_shared<block>(b);
    m_queue -> put(b_copy);
}
