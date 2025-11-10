#pragma once
#include "interfaces.h"
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>

class cmdParser
{
public:
    explicit cmdParser(std::shared_ptr<ICollector> collector, std::size_t N) : m_N(N), m_collector(collector) {};

    void parseIt(std::shared_ptr<ISource> from, bool endPresumption = true);
    void parseByString(const std::string&);
    void parseEnd();

    void setBlockSize(size_t s)
    {
	m_N = s;
    }

    void setThreadSave(bool state)
    {
	m_threadSave = state;
    }

    void setupNext(std::shared_ptr<cmdParser> n)
    {
	m_next = n;
    }

private:
    std::size_t m_N;
    std::shared_ptr<ICollector> m_collector;
    std::uint32_t m_counter{0};
    int m_dynamicBlockCounter{0};

    std::shared_ptr<cmdParser> m_next;
    std::mutex	m_lock;
    bool m_threadSave{false};

};
