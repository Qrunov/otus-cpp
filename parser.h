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
    explicit cmdParser(std::shared_ptr<ICollector> collector, std::size_t N) : m_N(N), m_collector(collector) {};

    void parseIt(std::shared_ptr<ISource> from, bool endPresumption = true);
    
    
    void parseByString(const std::string&);
    void parseEnd();


private:
    std::size_t m_N;
    std::shared_ptr<ICollector> m_collector;
    std::uint32_t counter{0};
    int dynamicBlockCounter{0};
};
