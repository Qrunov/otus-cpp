#pragma once
#include "interfaces.h"
#include <chrono>
#include <string>
#include <memory>

class openCollector : public collectorInterface
{
public:
    void beginBlock() override;
    void endBlock() override;
    void addCmd(const std::string &cmd) override;

    bool beginBlockWasCalled;
    bool endBlockWasCalled;
    std::string lastCmd;
};

void openCollector::beginBlock()
{
    beginBlockWasCalled = true;
}

void openCollector::endBlock()
{
    endBlockWasCalled = true;
}

void openCollector::addCmd(const std::string &cmd)
{
    lastCmd = cmd;
}

