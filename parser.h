#pragma once
#include "interfaces.h"
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

class cmdParser
{
public:
    explicit cmdParser(std::shared_ptr<collectorInterface> collector, std::size_t N) : m_N(N), m_collector(collector) {};

    bool parseIt(std::shared_ptr<sourceInterface> from, bool endPresumption = true);

private:
    std::size_t m_N;
    std::shared_ptr<collectorInterface> m_collector;
    std::uint32_t counter{0};
    int dynamicBlockCounter{0};
};
