#pragma once
#include <memory>
#include <map>
#include "async.h"
#include "async_context.h"
#include "opencollector.h"


namespace async {
extern std::map<handle_t, std::shared_ptr<async_context>> connections;
template <class Collector>
handle_t connect_t(std::size_t bulk);
std::shared_ptr<collectorInterface> getOpenCollector(handle_t handle)
{
    auto p_con = connections[handle];
    if (!p_con)
    {
	std::cerr << "no appropriate client handler has found" << std::endl;
	return nullptr;
    }

    return p_con -> m_collector;
}
void receiveEof(handle_t handle);
}