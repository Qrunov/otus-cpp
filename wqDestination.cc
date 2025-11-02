#include "wqDestination.h"
#include <memory>
#include <fstream>
#include <iostream>

using namespace std;
void wqDestination::update(const commandBlock &b)
{
    if (!m_queue)
    {
	cerr << "no destination query exists" << endl;
	return;
    }

    auto b_copy = make_shared<commandBlock>(b);
    m_queue -> put(b_copy);
}
