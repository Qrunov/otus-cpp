#pragma once
#include <memory>
#include "parser.h"
#include "collector.h"

namespace async {
struct async_context
{
    std::shared_ptr<cmdParser> m_parser;
    std::shared_ptr<collectorInterface> m_collector;
};
}
