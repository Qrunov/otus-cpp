#pragma once
#include "interfaces.h"
#include "collector.h"
#include <chrono>
#include <string>
#include <memory>

class collector : public collectorInterface, public subject
{
public:
    void beginBlock() override;
    void addCmd(const std::string &cmd) override;

    void endBlock() override;

private:
    std::shared_ptr<block> currentBlock;
};
