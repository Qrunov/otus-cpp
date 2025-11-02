#pragma once
#include "interfaces.h"
#include "collector.h"
#include <chrono>
#include <string>
#include <memory>

class collector : public ICollector, public subject
{
public:
    void beginBlock() override;
    void addCmd(const std::string &cmd) override;

    void endBlock() override;

private:
    std::shared_ptr<commandBlock> currentBlock;
};
