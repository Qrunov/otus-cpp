#pragma once
#include "interfaces.h"
#include <chrono>
#include <string>
#include <memory>

class openCollector : public ICollector
{
public:
    void beginBlock() override;
    void endBlock() override;
    void addCmd(const std::string &cmd) override;

    bool beginBlockWasCalled;
    bool endBlockWasCalled;
    std::string lastCmd;
};

